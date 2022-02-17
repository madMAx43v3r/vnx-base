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
#include <vnx/RecordReader.h>
#include <vnx/RecordPointer.hxx>

#include <algorithm>


namespace vnx {

RecordReader::RecordReader(const std::string& filename_)
	:	filename(filename_), in(&stream)
{
	file = fopen(filename.c_str(), "rb");
	if(!file) {
		throw std::runtime_error("fopen('" + filename + "', 'rb') failed with: " + std::string(::strerror(errno)));
	}
	stream.reset(file);
	reset();
}

RecordReader::~RecordReader() {
	if(file) {
		close();
	}
}

void RecordReader::reset() {
	vnx::fseek(file, 0, SEEK_SET);
	is_end_of_file = false;
	in.reset();
	header = std::dynamic_pointer_cast<RecordHeader>(vnx::read(in));
	if(!header) {
		throw std::runtime_error("invalid recording file: '" + filename + "' (!RecordHeader)");
	}
	read_index();
	if(index_pos < 0) {
		throw std::runtime_error("recording file is empty: '" + filename + "'");
	}
	// read all type codes first (so we don't skip them when seeking)
	auto type_code_positions = header->type_code_positions;
	std::sort(type_code_positions.begin(), type_code_positions.end(), std::less<uint64_t>());
	for(uint64_t pos : type_code_positions) {
		in.reset();
		vnx::fseek(file, pos, SEEK_SET);
		while(true) {
			uint16_t code;
			read(in, code);
			if(code == CODE_TYPE_CODE || code == CODE_ALT_TYPE_CODE) {
				read_type_code(in, &code);
			} else {
				break;
			}
		}
	}
	// read the first sample
	seek_and_read_sample();
}

void RecordReader::close() {
	::fclose(file);
	file = 0;
}

std::shared_ptr<const Sample> RecordReader::read_next() {
	if(!seek_next()) {
		return nullptr;
	}
	read_sample();
	return curr_sample;
}

std::shared_ptr<const Sample> RecordReader::read_prev() {
	if(!seek_prev()) {
		return nullptr;
	}
	read_sample();
	return curr_sample;
}

void RecordReader::seek_by_count(int64_t num_samples) {
	const bool forward = num_samples >= 0;
	while(num_samples != 0) {
		if(forward) {
			num_samples--;
			if(!seek_next()) {
				break;
			}
		} else {
			num_samples++;
			if(!seek_prev()) {
				break;
			}
		}
	}
	seek_and_read_sample();
}

void RecordReader::seek_by_time(int64_t num_micros) {
	const bool forward = num_micros >= 0;
	const int64_t base_time = get_time_micros();
	while(true) {
		const int64_t delta = get_time_micros() - base_time;
		if(forward) {
			if(delta >= num_micros || !seek_next()) {
				break;
			}
		} else {
			if(delta <= num_micros || !seek_prev()) {
				break;
			}
		}
	}
	seek_and_read_sample();
}

void RecordReader::seek_to_time(int64_t time_micros) {
	seek_by_time(time_micros - get_time_micros());
}

void RecordReader::seek_to_position(double position) {
	seek_to_time(int64_t(header->begin_time + position * (header->end_time - header->begin_time)));
}

int64_t RecordReader::get_time_micros() const {
	return curr_block->index[index_pos].time;
}

int64_t RecordReader::get_input_pos() const {
	return vnx::ftell(file) - in.get_num_avail();
}

std::shared_ptr<Sample> RecordReader::get_sample() const {
	return curr_sample;
}

std::shared_ptr<const RecordHeader> RecordReader::get_header() const {
	return header;
}

bool RecordReader::begin_of_file() const {
	return is_begin_of_file;
}

bool RecordReader::end_of_file() const {
	return is_end_of_file;
}

bool RecordReader::seek_next() {
	is_begin_of_file = false;
	is_end_of_file = false;
	index_pos++;
	if(index_pos >= int32_t(curr_block->index.size())) {
		if(curr_block->next_block >= 0) {
			vnx::fseek(file, curr_block->next_block, SEEK_SET);
			in.reset();
			read_index();
			// now we already point to the first sample of the next block
			// check for special case where last block is empty
			if(index_pos < 0) {
				fix_index_pos();
				is_end_of_file = true;
				return false;
			}
		} else {
			fix_index_pos();
			is_end_of_file = true;
			return false;
		}
	}
	return true;
}

bool RecordReader::seek_prev() {
	is_begin_of_file = false;
	is_end_of_file = false;
	index_pos--;
	if(index_pos < 0) {
		if(curr_block->prev_block >= 0) {
			vnx::fseek(file, curr_block->prev_block, SEEK_SET);	// go to the previous block
			in.reset();
			read_index();
			index_pos = curr_block->index.size() - 1;	// and to the last sample of the previous block
			seek_and_read_sample();
		} else {
			fix_index_pos();
			is_begin_of_file = true;
			return false;
		}
	}
	return true;
}

void RecordReader::fix_index_pos() {
	if(curr_block->index.empty()) {
		if(curr_block->prev_block >= 0) {
			// in this special case where the last block is empty we go back to the second last block
			vnx::fseek(file, curr_block->prev_block, SEEK_SET);
			in.reset();
			read_index();
			index_pos = curr_block->index.size() - 1;	// and to the last sample of that second last block
			seek_and_read_sample();
		} else {
			throw std::logic_error("RecordReader: recording is empty!");
		}
	} else if(index_pos < 0) {
		index_pos = 0;
	} else if(index_pos >= int32_t(curr_block->index.size())) {
		index_pos = curr_block->index.size() - 1;
	}
}

void RecordReader::seek_and_read_sample() {
	vnx::fseek(file, curr_block->index[index_pos].pos, SEEK_SET);
	in.reset();
	read_sample();
}

void RecordReader::read_sample() {
	std::shared_ptr<Value> value;
	try {
		value = vnx::read(in);
	} catch(...) {
		// oh well
	}
	if(value) {
		if(auto pointer = std::dynamic_pointer_cast<RecordPointer>(value)) {
			const auto prev = in.get_input_pos();
			vnx::fseek(file, pointer->position, SEEK_SET);
			in.reset();
			read_sample();
			vnx::fseek(file, prev, SEEK_SET);
			in.reset();
		} else if(auto sample = std::dynamic_pointer_cast<Sample>(value)) {
			curr_sample = sample;
		} else {
			throw std::runtime_error("invalid recording file: '" + filename + "' (!Sample)");
		}
	} else {
		is_end_of_file = true;
	}
}

void RecordReader::read_index() {
	curr_block = std::dynamic_pointer_cast<RecordIndex>(vnx::read(in));
	if(!curr_block) {
		throw std::runtime_error("invalid recording file: '" + filename + "' (!RecordIndex)");
	}
	// remove empty index entries
	while(!curr_block->index.empty() && curr_block->index.back().pos == int64_t(-1)) {
		curr_block->index.pop_back();
	}
	if(curr_block->index.empty()) {
		index_pos = -1;
	} else {
		index_pos = 0;
	}
}


} // vnx
