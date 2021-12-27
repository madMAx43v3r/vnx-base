/*
 * TerminalInput.h
 *
 *  Created on: Sep 14, 2020
 *      Author: jaw
 */


#include <vnx/TerminalInput.h>



namespace vnx{

TerminalInput::TerminalInput():
	m_historyPos(m_history.begin())
{}


void TerminalInput::add_history(const std::string &line){
	m_history.push_back(line);
	m_historyPos = m_history.end();
}


std::string TerminalInput::history_up(){
	if(m_historyPos != m_history.begin()){
		m_historyPos--;
	}

	if(m_historyPos != m_history.end()) return *m_historyPos;
	return "";
}


std::string TerminalInput::history_down(){
	if(m_historyPos != m_history.end()) m_historyPos++;

	if(m_historyPos != m_history.end()) return *m_historyPos;
	return "";
}


void TerminalInput::history_reset(){
	m_historyPos = m_history.end();
}


void TerminalInput::add_command(const std::string &command){
	m_commands.insert(command);
}


void TerminalInput::add_argument(const std::string &command, const std::string &argument){
	m_arguments[command].insert(argument);
}


void TerminalInput::clear_commands(){
	m_commands.clear();
	m_arguments.clear();
}


void TerminalInput::clear_arguments(const std::string &command){
	m_arguments.erase(command);
}


std::pair<std::string, bool> TerminalInput::complete_command(const std::string &prefix) const{
	return complete(prefix, m_commands);
}


std::pair<std::string, bool> TerminalInput::complete_argument(const std::string &command, const std::string &prefix) const{
	auto found = m_arguments.find(command);
	if(found == m_arguments.end()) return std::make_pair("", false);
	return complete(prefix, found->second);
}


std::pair<std::string, bool> TerminalInput::complete(const std::string &prefix, const std::set<std::string> &set) const{
	std::string result = "";
	size_t diffpos = 0;

	auto lower = set.lower_bound(prefix);
	bool unique = true;
	for(auto iter=lower; iter!=set.end(); iter++){
		const std::string &i = *iter;
		if(i.rfind(prefix, 0) == 0){
			if(result.empty()){
				result = i;
				diffpos = i.length();
			}else{
				size_t dp = string_differ(result, i);
				if(dp < diffpos) diffpos = dp;
				unique = false;
			}
		}
	}

	if(result == "") return std::make_pair(result, false);
	return std::make_pair(result.substr(prefix.length(), diffpos-prefix.length()), unique);
}


std::vector<std::string> TerminalInput::guess_command(const std::string &prefix) const{
	return guess(prefix, m_commands);
}


std::vector<std::string> TerminalInput::guess_argument(const std::string &command, const std::string &prefix) const{
	auto found = m_arguments.find(command);
	if(found == m_arguments.end()) return {};
	return guess(prefix, found->second);
}


std::vector<std::string> TerminalInput::guess(const std::string &prefix, const std::set<std::string> &set) const{
	std::vector<std::string> result;

	auto lower = set.lower_bound(prefix);
	for(auto iter=lower; iter!=set.end(); iter++){
		const std::string &found = *iter;
		if(found.rfind(prefix, 0) != 0) break;
		result.push_back(found);
	}

	return result;
}


size_t TerminalInput::string_differ(const std::string &one, const std::string &two){
	size_t endpos = std::min(one.length(), two.length());
	for(size_t i=0; i<endpos; i++){
		if(one[i] != two[i]) return i;
	}
	return endpos;
}



} // vnx




