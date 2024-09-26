#pragma once
#include <string>
#include "../../dependencies/physfs/include/physfs.h"

class ZobFilePath
{

public:
	enum eFileType
	{
		eFileType_all=0,
		eFileType_mesh=1,
		eFileType_texture=2,
		eFileType_xml=3,
		__eFileType_ALL__
	};

	ZobFilePath();
	ZobFilePath(std::string name, std::string path, std::string file, bool bAbsolute);
	ZobFilePath(std::string name, char* buffer, long len);
	~ZobFilePath();
	bool						IsDefined();
	std::string					GetFullPath();
	std::string					GetFullPathWithoutFile();
	std::string					Serialize();
	void						Unserialize(std::string s);
	void						Reset();
	
	eFileType					GetFileType() const { return m_fileType; }
	void						SetFileType(eFileType t) { m_fileType = t; }
	inline const std::string&	GetName() const { return m_name; }
	inline const std::string&	GetFile() const { return m_file; }
	inline const std::string&	GetPath() const { return m_path; }
	inline const bool			IsAbsolute() const { return m_bAbsolute; }

	void						LoadData();
	char*						GetData() const { return m_buf; }
	long						GetDataLength() const { return m_bufLen; }
	void						UnloadData();
private:
	std::string					m_name;
	std::string					m_path;
	std::string					m_file;
	bool						m_bAbsolute;
	eFileType					m_fileType;
	char*						m_buf;
	long						m_bufLen;
	bool						m_freeBuffer;
};