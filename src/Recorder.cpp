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
#include <vnx/Recorder.h>


namespace vnx {

Recorder::Recorder(const std::string& _vnx_name)
	:	RecorderBase(_vnx_name)
{
	vnx_auto_decompress = false;	// record compressed values
}

void Recorder::init() {
	if(receive_tunnel) {
		open_pipe(receive_tunnel, this, max_queue_ms);
	}
	if(!proxy_name.empty()) {
		proxy = std::make_shared<ProxyClient>(proxy_name);
	}
	for(const std::string& topic : topics) {
		if(!receive_tunnel) {
			subscribe(topic, max_queue_ms);	// in case of a tunnel we don't need to subscribe 
		}
		if(proxy) {
			proxy->enable_import(topic);
		}
	}
}

void Recorder::main() {
	if(topics.empty()) {
		log(ERROR) << "no topics to record!";
		return;
	}
	
	set_timer_millis(flush_interval_ms, std::bind(&Recorder::flush, this));
	set_timer_millis(1000, std::bind(&Recorder::print_stats, this));
	
	if(filename.empty()) {
		filename = "record_%.dat";
	}
	if(filename.back() == '/') {
		filename += "record_%.dat";
	}
	filename = vnx::string_subs(filename, "%", vnx::get_date_string_ex("%Y%m%d_%H%M%S"));
	
	info.file_name = filename;
	info.is_recording = true;
	
	log(INFO) << "Writing to '" << filename << "'";
	{
		writer = std::make_shared<RecordWriter>(filename, header_size, block_size);
		Super::main();
		log(INFO) << "Closing file, please wait ...";
		writer->close();
	}
	log(INFO) << "File closed";
	
	if(proxy) {
		for(const std::string& topic : topics) {
			try {
				proxy->disable_import(topic);
			} catch(...) {
				break;
			}
		}
	}
}

void Recorder::handle(std::shared_ptr<const Sample> sample) {
	Super::handle(sample);
	if(sample->value) {
		writer->write(sample);
	}
}

void Recorder::handle_resend(std::shared_ptr<const Sample> sample) {
	handle(sample);
}

void Recorder::flush() {
	writer->flush();
}

void Recorder::print_stats() {
	const RecordHeader& header = writer->get_header();
	info.bandwidth = double(writer->get_output_pos() - last_output_pos) / 1024 / 1024;
	info.sample_rate = header.num_samples - last_sample_count;
	info.loss_factor = 100 * header.num_samples_lost / double(header.num_samples + header.num_samples_lost);
	info.header = header;
	publish(info, output_status);
	
	log(INFO) << (header.num_bytes / 1024 / 1024) << " MB, " << info.bandwidth << " MB/s, "
			<< int64_t(info.sample_rate) << " s/s, " << info.loss_factor << " % lost";
	
	last_output_pos = writer->get_output_pos();
	last_sample_count = header.num_samples;
}


} // vnx
