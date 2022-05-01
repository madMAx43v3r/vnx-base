 /*************************************************************************
 * 
 *  [2017] - [2018] Automy Inc. 
 *  All Rights Reserved.
 * 
 * NOTICE:  All information contained herein is, and remains
 * the property of Automy Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Automy Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Automy Incorporated.
 */

#include <vnx/vnx.h>
#include <vnx/RecordWriter.h>
#include <vnx/RecordPointer.hxx>
#include <vnx/Marker.hxx>

#include <algorithm>


namespace vnx {

RecordWriter::RecordWriter(const std::string& filename, uint32_t header_size, uint32_t block_size)
	:	out(&stream)
{
	file = fopen(filename.c_str(), "wb");
	if(!file) {
		throw std::runtime_error("fopen('" + filename + "', 'wb') failed with: " + std::string(::strerror(errno)));
	}
	stream.reset(file);
	
	vnx::write(out, uint16_t(CODE_NONE));		// vnx file header
	vnx::write(out, uint16_t(CODE_MAGIC));		// vnx file header
	
	// force write the type codes first so they don't get in the way later
	out.write_type_code(RecordHeader::static_get_type_code());
	out.write_type_code(RecordIndex::static_get_type_code());
	
	// store position where we write the header
	header_start_pos = out.get_output_pos();
	
	// write header
	header.header_size = header_size;
	header.block_size = block_size;
	header.filename = filename;
	write_header();
	
	// start first block by writing it's index
	curr_block = RecordIndex::create();
	curr_block->index.resize(block_size);
	curr_block_pos = header_size;
	write_index();
}

RecordWriter::~RecordWriter() {
	if(file) {
		close();
	}
}

void RecordWriter::write(std::shared_ptr<const Sample> sample) {
	// we use the time when the sample was received by this process
	auto time = sample->recv_time;
	if(time < last_sample_time) {
		time = last_sample_time;	// make sure the timestamps are strictly increasing
	}
	if(!header.begin_time) {
		header.begin_time = time;
	}
	last_sample_time = time;
	
	// update the index
	record_index_entry_t& entry = curr_block->index[index_pos++];
	entry.pos = out.get_output_pos();
	entry.time = time;
	
	const auto topic_hash = sample->topic->get_hash();
	record_topic_info_t& topic_info = topic_map[sample->topic];
	channel_t& channel = channel_map[std::make_pair(sample->src_mac, topic_hash)];

	// check for resent messages
	if(sample->seq_num > channel.seq_num || channel.output_pos < 0) {
		// write sample to file
		vnx::write(out, sample);
		channel.output_pos = entry.pos;
		topic_info.num_bytes += out.get_output_pos() - entry.pos;
	} else {
		// point back to last data
		RecordPointer pointer;
		pointer.position = channel.output_pos;
		vnx::write(out, pointer);
	}
	
	// update statistics
	topic_info.name = sample->topic->get_name();
	topic_info.num_samples++;
	
	// check sequence number to see if we lost samples
	if(channel.seq_num > 0 && sample->seq_num > channel.seq_num + 1) {
		const uint64_t num_lost = (sample->seq_num - channel.seq_num) - 1;
		topic_info.num_samples_lost += num_lost;
	}
	channel.seq_num = sample->seq_num;

	// check if we got a marker
	if(std::dynamic_pointer_cast<const Marker>(sample->value)) {
		header.markers.push_back(entry.pos);	// add it to the header for convenience
	}
	
	// check if current block is now full
	if(index_pos >= int32_t(header.block_size)) {
		add_block();	// if yes, add a new block
	}
}

void RecordWriter::flush() {
	write_index();		// re-write current index
	write_header();		// re-write header
	::fflush(file);
	vnx::fseek(file, 0, SEEK_END);		// go back to the end
}

void RecordWriter::close() {
	flush();
	::fclose(file);
	file = nullptr;
}

const RecordHeader& RecordWriter::get_header() const {
	return header;
}

int64_t RecordWriter::get_output_pos() const {
	return out.get_output_pos();
}

void RecordWriter::write_header() {
	// update header
	header.end_time = last_sample_time;

	// update type code positions
	header.type_code_positions.clear();
	for(const auto& entry : out.type_code_map) {
		header.type_code_positions.push_back(entry.second);
	}
	std::sort(header.type_code_positions.begin(), header.type_code_positions.end(), std::less<uint64_t>());

	// update topic statistics
	header.topics.clear();
	header.num_bytes = 0;
	header.num_samples = 0;
	header.num_samples_lost = 0;
	for(const auto& entry : topic_map) {
		const auto& topic = entry.second;
		header.topics.push_back(topic);
		header.num_bytes += topic.num_bytes;
		header.num_samples += topic.num_samples;
		header.num_samples_lost += topic.num_samples_lost;
	}

	// write header
	out.flush();	// make sure buffer is empty before seek
	vnx::fseek(file, header_start_pos, SEEK_SET);
	vnx::write(out, header);
	
	const auto actual_header_size = out.get_output_pos();
	if(actual_header_size + 6 > header.header_size) {
		throw std::runtime_error("record header overflow");
	}
	vnx::write_padding(out, header.header_size - actual_header_size);
	out.flush();	// make sure we leave no data in buffer
}

void RecordWriter::write_index() {
	out.flush();	// make sure buffer is empty before seek
	vnx::fseek(file, curr_block_pos, SEEK_SET);
	vnx::write(out, curr_block);
	out.flush();	// make sure we leave no data in buffer
}

void RecordWriter::add_block() {
	// first re-write the current block since it's now finished
	auto next_block_pos = out.get_output_pos();
	curr_block->next_block = next_block_pos;	// link it to the next block
	write_index();
	
	// create index for the new block
	auto next_block = RecordIndex::create();
	next_block->prev_block = curr_block_pos;	// link it to the previous block
	next_block->index.resize(header.block_size);	// fill it up with dummy data
	
	// write index for the new block to file
	vnx::fseek(file, 0, SEEK_END);
	vnx::write(out, next_block);
	out.flush();
	
	// update our state
	curr_block_pos = next_block_pos;
	curr_block = next_block;
	index_pos = 0;
}


} // vnx
