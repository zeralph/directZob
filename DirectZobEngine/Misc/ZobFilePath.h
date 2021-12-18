#pragma once
#include <string>

class ZobFilePath
{

public:
	ZobFilePath();
	ZobFilePath(std::string name, std::string path, std::string file, bool bAbsolute);
	~ZobFilePath();
	bool						IsDefined();
	std::string					GetFullPath();
	std::string					GetFullPathWithoutFile();
	std::string					Serialize();
	void						Unserialize(std::string s);
	void						Reset();
	inline const std::string	GetName() const { return m_name; }
	inline const std::string	GetFile() const { return m_file; }
	inline const std::string	GetPath() const { return m_path; }
	inline const bool			IsAbsolute() const { return m_bAbsolute; }
private:
	std::string					m_name;
	std::string					m_path;
	std::string					m_file;
	bool						m_bAbsolute;
};