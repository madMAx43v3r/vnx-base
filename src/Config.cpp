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

#include <mutex>
#include <memory>
#include <fstream>
#include <iostream>


namespace vnx {

class GlobalConfig {
public:
	Variant get_config(const std::string& key, bool protect) const {
		std::lock_guard<std::mutex> lock(mutex);
		if(protect && is_protected_internal(key)) {
			throw std::runtime_error("config is protected: '" + key + "'");
		}
		auto iter = config_map.find(key);
		if(iter != config_map.end()) {
			return iter->second;
		}
		const auto object = get_config_object_internal(key, protect);
		if(!object.empty()) {
			return Variant(object);
		}
		return Variant();
	}
	
	Object get_config_object(const std::string& key, bool protect) const {
		std::lock_guard<std::mutex> lock(mutex);
		return get_config_object_internal(key, protect);
	}

	void set_config(const std::string& key, const Variant& value, bool protect) {
		std::lock_guard<std::mutex> lock(mutex);
		if(protect && is_protected_internal(key)) {
			throw std::runtime_error("config is protected: '" + key + "'");
		}
		config_map[key] = value;
	}

	std::vector<std::pair<std::string, Variant>> get_all_configs(bool protect) const {
		std::lock_guard<std::mutex> lock(mutex);
		std::vector<std::pair<std::string, Variant>> out;
		for(const auto& entry : config_map) {
			if(!protect || !is_protected_internal(entry.first)) {
				out.push_back(entry);
			}
		}
		return out;
	}

	void add_protection(const std::string& key) {
		std::lock_guard<std::mutex> lock(mutex);
		protection.insert(key);
	}

	bool is_protected(const std::string& key) const {
		std::lock_guard<std::mutex> lock(mutex);
		return is_protected_internal(key);
	}

	std::set<std::string> get_protection() const {
		std::lock_guard<std::mutex> lock(mutex);
		return protection;
	}

private:
	Object get_config_object_internal(const std::string& path, bool protect) const {
		Object object;
		for(const auto& entry : config_map) {
			const auto& key = entry.first;
			if(key.size() > path.size()
				&& (path.empty() || key[path.size()] == '.')
				&& key.substr(0, path.size()) == path)
			{
				if(key.find_last_of('.') == path.size()) {
					if(!protect || !is_protected_internal(key)) {
						const auto field = key.substr(path.size() + 1);
						object[field] = entry.second;
					}
				} else {
					const size_t start = path.empty() ? 0 : path.size() + 1;
					const auto next_dot = key.find('.', start);

					std::string field;
					if(next_dot == std::string::npos){
						field = key.substr(start);
					}else{
						const size_t stop = path.empty() ? next_dot : next_dot - path.size() - 1;
						field = key.substr(start, stop);
					}
					auto &fill_me = object[field];
					if(fill_me.empty()){
						if(next_dot == std::string::npos){
							fill_me = entry.second;
						}else{
							const auto next_key = key.substr(0, next_dot);
							fill_me = get_config_object_internal(next_key, protect);
						}
					}
				}
			}
		}
		return object;
	}
	
	bool is_protected_internal(const std::string& key) const {
		std::string key_ = key;
		while(!key_.empty()) {
			if(protection.find(key_) != protection.end()) {
				return true;
			}
			const auto pos = key_.find_last_of('.');
			if(pos != std::string::npos) {
				key_.resize(pos);
			} else {
				break;
			}
		}
		return false;
	}

private:
	mutable std::mutex mutex;
	std::set<std::string> protection;
	std::map<std::string, Variant> config_map;
	
};


static GlobalConfig& get_global_config() {
	static GlobalConfig config;
	return config;
}

Variant get_config(const std::string& key, bool protect) {
	return get_global_config().get_config(key, protect);
}

Object get_config_object(const std::string& name_space, bool protect) {
	return get_global_config().get_config_object(name_space, protect);
}

std::vector<std::pair<std::string, Variant>> get_all_configs(bool protect) {
	return get_global_config().get_all_configs(protect);
}

void set_config(const std::string& key, const Variant& value, bool protect) {
	if(value.is_object()) {
		for(const auto& field : value.to_object().field) {
			const auto key_ = key + ((key.empty() || key.back() == '.') ? "" : ".") + field.first;
			set_config(key_, field.second, protect);
		}
	} else if(!key.empty() && key.back() == '+') {
		auto key_ = key;
		key_.pop_back();
		auto vector = get_config(key_).to<std::vector<Variant>>();
		if(value.is_array()) {
			const auto more = value.to<std::vector<Variant>>();
			vector.insert(vector.end(), more.begin(), more.end());
		} else {
			vector.emplace_back(value);
		}
		get_global_config().set_config(key_, Variant(vector), protect);
	} else {
		get_global_config().set_config(key, value, protect);
	}
}

void set_config(const std::string& key, const std::string& value, bool protect) {
	Variant tmp;
	vnx::from_string(value, tmp);
	set_config(key, tmp, protect);
}

void protect_config(const std::string& key) {
	get_global_config().add_protection(key);
}

bool is_config_protected(const std::string& key) {
	return get_global_config().is_protected(key);
}

std::set<std::string> get_config_protection() {
	return get_global_config().get_protection();
}

static void read_config_tree(const Directory& current, const std::string& config_path);

void read_config_tree(const std::string& root_path) {
	if(root_path.empty()) {
		return;
	}
	Directory root(root_path);
	root.open();
	{
		// try to find parent
		std::ifstream parent(root.get_path() + "parent");
		if(parent.good()) {
			std::string path;
			vnx::read(parent, path);
			if(!path.empty() && path != ".") {
				read_config_tree((path.find_first_of("/\\") != 0 ? root.get_path() : "") + path);
			}
		}
	}
	// parse the whole tree
	read_config_tree(root, "");
}

void read_config_tree(const Directory& current, const std::string& config_path) {
	for(auto& dir : current.directories()) {
		dir->open();
		read_config_tree(*dir, config_path + dir->get_name() + ".");
	}
	for(auto& file : current.files()) {
		const auto file_name = file->get_name(false);
		const auto file_extension = file->get_extension();
		if(file_name.empty() || file_name.front() == '.' || file_name.back() == '.') {
			continue;
		}
		try {
			std::ifstream stream(file->get_path());
			if(file_extension == ".jary") {
				std::vector<Variant> array;
				for(std::string line; std::getline(stream, line);) {
					std::istringstream ss(line);
					if(auto json = vnx::read_json(ss, true)) {
						array.emplace_back(json->to_variant());
					}
				}
				set_config(config_path + file_name, Variant(array));
			} else if(file_extension == ".lst") {
				std::vector<Variant> array;
				for(std::string line; std::getline(stream, line);) {
					array.emplace_back(Variant(line));
				}
				set_config(config_path + file_name, Variant(array));
			} else if(file_extension == ".txt") {
				const std::string text((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
				set_config(config_path + file_name, Variant(text));
			} else if(file_extension.empty() || file_extension == ".json") {
				if(auto json = vnx::read_json(stream, true)) {
					set_config(config_path + file_name, json->to_variant());
				}
			}
		} catch(const std::exception& ex) {
			std::cerr << "Failed to read config '" << file->get_path() << "': " << ex.what() << std::endl;
			throw;
		}
	}
}

vnx::Object read_config_file(const std::string& file_path) {
	std::ifstream stream(file_path);
	if(auto json = vnx::read_json(stream, true)) {
		return json->to_object();
	}
	return vnx::Object();
}

void write_config_file(const std::string& file_path, const vnx::Object& config) {
	std::ofstream stream(file_path, std::ios::out);
	vnx::PrettyPrinter out(stream);
	vnx::accept(out, config);
}


} //vnx
