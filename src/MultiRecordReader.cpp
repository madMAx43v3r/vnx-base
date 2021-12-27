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
#include <vnx/MultiRecordReader.h>


namespace vnx {

MultiRecordReader::MultiRecordReader(const std::vector<std::string>& filenames_) {
	
	// create a normal RecordReader for each file
	for(const std::string& filename : filenames_) {
		try {
			readers.push_back(std::make_shared<RecordReader>(filename));
		} catch(...) {
			// ignore
		}
	}
	
	// merge the headers into a combined header
	int i = 0;
	std::map<std::string, record_topic_info_t> topic_map;
	header = RecordHeader::create();
	header->filename = "[";
	for(const auto& reader : readers) {
		std::shared_ptr<const RecordHeader> h = reader->get_header();
		if(i++ > 0) {
			header->filename += ",";
		}
		header->filename += h->filename;
		if(h->begin_time < header->begin_time || !header->begin_time) {
			header->begin_time = h->begin_time;
		}
		if(h->end_time > header->end_time || !header->end_time) {
			header->end_time = h->end_time;
		}
		header->num_bytes += h->num_bytes;
		header->num_samples += h->num_samples;
		header->num_samples_lost += h->num_samples_lost;
		for(const record_topic_info_t& t : h->topics) {
			record_topic_info_t& topic = topic_map[t.name];
			topic.name = t.name;
			topic.num_bytes += t.num_bytes;
			topic.num_samples += t.num_samples;
			topic.num_samples_lost += t.num_samples_lost;
		}
	}
	for(const auto& entry : topic_map) {
		header->topics.push_back(entry.second);
	}
	header->filename += "]";
	
	// initialize with the first sample
	reset();
}

MultiRecordReader::~MultiRecordReader() = default;

void MultiRecordReader::reset() {
	for(const auto& reader : readers) {
		reader->reset();
	}
	read_next();
	if(!curr_file) {
		throw std::logic_error("MultiRecordReader::reset(): !curr_file");
	}
	if(!curr_sample) {
		throw std::logic_error("MultiRecordReader::reset(): !curr_sample");
	}
}

void MultiRecordReader::close() {
	for(const auto& reader : readers) {
		reader->close();
	}
}

std::shared_ptr<const Sample> MultiRecordReader::read_next() {
	int64_t found = -1;
	for(const auto& reader : readers) {
		// find the file which has the next closest sample
		if(!reader->end_of_file()) {
			const int64_t time = reader->get_time_micros();
			if(time < found || found < 0) {
				found = time;
				curr_file = reader;
			}
		}
	}
	if(found >= 0) {
		curr_sample = curr_file->get_sample();		// get the next sample (which is now the current)
		curr_time = curr_file->get_time_micros();	// update the current time
		curr_file->read_next();						// already read the next sample
		return curr_sample;
	}
	// in this case we are at the end of all files
	return 0;
}

std::shared_ptr<const Sample> MultiRecordReader::read_prev() {
	int64_t found = -1;
	for(const auto& reader : readers) {
		// find the file which has the next closest sample
		if(!reader->begin_of_file()) {
			const int64_t time = reader->get_time_micros();
			if(time > found || found < 0) {
				found = time;
				curr_file = reader;
			}
		}
	}
	if(found >= 0) {
		curr_sample = curr_file->get_sample();		// get the next sample (which is now the current)
		curr_time = curr_file->get_time_micros();	// update the current time
		curr_file->read_prev();						// already read the next sample
		return curr_sample;
	}
	// in this case we are at the beginning of all files
	return 0;
}

void MultiRecordReader::seek_by_count(int64_t num_samples) {
	if(num_samples > 0) {
		for(int64_t i = 0; i < num_samples; ++i) {
			if(!read_next()) {
				break;
			}
		}
	} else {
		for(int64_t i = num_samples; i < 0; ++i) {
			if(!read_prev()) {
				break;
			}
		}
	}
}

void MultiRecordReader::seek_by_time(int64_t num_micros) {
	// just transform the request to an absolute seek
	seek_to_time(curr_time + num_micros);
}

void MultiRecordReader::seek_to_time(int64_t time_micros) {
	const bool forward = time_micros >= curr_time;
	// try to seek all files to the requested time
	for(const auto& reader : readers) {
		reader->seek_to_time(time_micros);
	}
	// now find the file which is the closest to the requested time
	curr_file = 0;
	{
		int64_t min_diff = 0;
		for(const auto& reader : readers) {
			if(forward && reader->end_of_file()) {
				continue;				// end of file does not count in forward mode
			}
			if(!forward && reader->begin_of_file()) {
				continue;				// begin of file does not count in backward mode
			}
			int64_t diff = reader->get_time_micros() - time_micros;
			if(diff < 0) {
				diff = -diff;
			}
			if(!curr_file || diff < min_diff) {
				min_diff = diff;
				curr_file = reader;
			}
		}
	}
	if(!curr_file) {
		// in this case we are either at the end or beginning of all files
		int64_t min_max = 0;
		for(const auto& reader : readers) {
			int64_t time = reader->get_time_micros();
			if(forward) {
				if(!curr_file || time > min_max) {
					min_max = time;			// take the last possible sample
					curr_file = reader;
				}
			} else {
				if(!curr_file || time < min_max) {
					min_max = time;			// take the first possible sample
					curr_file = reader;
				}
			}
		}
	}
	curr_sample = curr_file->get_sample();			// update the sample pointer to the new position
	curr_time = curr_file->get_time_micros();		// update the current time also
}

void MultiRecordReader::seek_to_position(double position) {
	// transform the request to an absolute seek
	seek_to_time(header->begin_time + int64_t(position * (header->end_time - header->begin_time)));
}

int64_t MultiRecordReader::get_time_micros() const {
	return curr_time;
}

int64_t MultiRecordReader::get_input_pos() const {
	int64_t pos = 0;
	for(const auto& reader : readers) {
		pos += reader->get_input_pos();			// sum of all files
	}
	return pos;
}

std::shared_ptr<Sample> MultiRecordReader::get_sample() const {
	return curr_sample;
}

std::shared_ptr<const RecordHeader> MultiRecordReader::get_header() const {
	return header;
}


} // vnx
