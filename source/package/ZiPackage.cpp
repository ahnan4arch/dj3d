#include "zip.h"
#include "unzip.h"
#include "ZiPackage.h"
#include <assert.h>
#include <time.h>

namespace dj3d
{
<<<<<<< HEAD
	static uLong GetDosDateTime(unsigned short mday, unsigned short month, unsigned short year, 
		unsigned short sec, unsigned short min, unsigned short hour)
	{
		uLong dosDateTime = 0;
		*((unsigned short*)&dosDateTime + 1) = (mday | ((month + 1) << 5) | ((year - 80) << 9));
		*((unsigned short*)&dosDateTime + 0) = ((sec / 2) | (min << 5) | (hour << 11));
		return dosDateTime;
	}

	class ZiPackageInternal
	{
	public:
		explicit ZiPackageInternal(const std::string& strZipName);
		bool CreateFile(const std::string& strFilePath);
		bool WriteBulk(const MemoryBlock& blk);
		bool FlushFile(void);
		bool Close(void);
		virtual ~ZiPackageInternal(void);
	private:
		zipFile m_zf;
		zip_fileinfo* m_zi;
		zip_fileinfo  m_zi_info;
	}; // class ZiPackageInternal

	ZiPackageInternal::ZiPackageInternal(const std::string& strZipName):
	m_zf( zipOpen64( strZipName.c_str(), APPEND_STATUS_CREATE) ),
		m_zi( NULL )
	{
		assert( NULL != m_zf );
		return;
	}

	bool ZiPackageInternal::CreateFile(const std::string& strFilePath)
	{
		if (NULL != m_zi)
		{
			return false;
		}

		m_zi = &m_zi_info;
		memset( m_zi, 0, sizeof(*m_zi) );
		m_zi->external_fa = 32;

		time_t now;
		time(&now);
		tm* local_time = localtime(&now);

		m_zi->tmz_date.tm_sec = local_time->tm_sec;
		m_zi->tmz_date.tm_min = local_time->tm_min;
		m_zi->tmz_date.tm_hour = local_time->tm_hour;
		m_zi->tmz_date.tm_mday = local_time->tm_mday;
		m_zi->tmz_date.tm_mon = local_time->tm_mon;
		m_zi->tmz_date.tm_year = local_time->tm_year;

		// dos date
		m_zi->dosDate = GetDosDateTime( local_time->tm_mday, 
			local_time->tm_mon, 
			local_time->tm_year, 
			local_time->tm_sec, 
			local_time->tm_min, 
			local_time->tm_hour );

		return ZIP_OK == zipOpenNewFileInZip3_64( m_zf, strFilePath.c_str(), m_zi, 
			NULL, 0, NULL, 0, NULL, 
			Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0,
			-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, 
			NULL, 0, 0 );
	}

	bool ZiPackageInternal::WriteBulk(const MemoryBlock& blk)
	{
		if (NULL == m_zi)
		{
			return false;
		}

		if ( 0 != blk.size() )
		{
			return zipWriteInFileInZip( m_zf, blk.offsetT<unsigned char>(0), static_cast<unsigned int>(blk.size()) ) >= 0;
		}
		else
		{
			return true;
		}
	}

	bool ZiPackageInternal::FlushFile(void)
	{
		if (NULL == m_zi)
		{
			return false;
		}

		bool bResult = (ZIP_OK == zipCloseFileInZip(m_zf));
		m_zi = NULL;

		return bResult;
	}

	bool ZiPackageInternal::Close(void)
	{
		assert( NULL == m_zi );

		if (NULL != m_zf)
		{
			bool bRtn = (ZIP_OK == zipClose(m_zf, NULL));
			m_zf = NULL;
			return bRtn;
		}
		else
		{
			return false;
		}
	}

	ZiPackageInternal::~ZiPackageInternal(void)
	{
		Close();
		return;
	}

	ZiPackage::ZiPackage(const std::string& strZipName):
	m_internal( new ZiPackageInternal(strZipName) )
	{
		return;
	}

	bool ZiPackage::CreateFile(const std::string& strFilePath)
	{
		if (NULL == m_internal)
		{
			return false;
		}
		else
		{
			return m_internal->CreateFile(strFilePath);
		}
	}

	bool ZiPackage::WriteBulk(const MemoryBlock& blk)
	{
		if (NULL == m_internal)
		{
			return false;
		}
		else
		{
			return m_internal->WriteBulk(blk);
		}
	}

	bool ZiPackage::FlushFile(void)
	{
		if (NULL == m_internal)
		{
			return false;
		}
		else
		{
			return m_internal->FlushFile();
		}
	}

	bool ZiPackage::Close(void)
	{
		if (NULL == m_internal)
		{
			return false;
		}
		else
		{
			return m_internal->Close();
		}
	}

	ZiPackage::~ZiPackage(void)
	{
		if (NULL != m_internal)
		{
			delete m_internal;
			m_internal = NULL;
		}

		return;
	}

	class UnziPackageInternal
	{
	public:
		UnziPackageInternal(const std::string& strZippedFileName);
		bool NextFileInZip(std::string& strFileName);
		bool FindFileInZip(const std::string& strFileName);
		bool OpenFileInZip(void);
		bool ReadBulk(MemoryBlock& blk, size_t& validLength);
		bool ReadBulk(MemoryBlock& blk);
		bool CloseFileInZip(void);
		void Rewind(void);
		virtual ~UnziPackageInternal(void);
	private:
		ZPOS64_T m_cur;
		unz_global_info64 m_gi;
		unzFile m_uf;
	}; // class UnziPackageInternal

	UnziPackageInternal::UnziPackageInternal(const std::string& strZippedFileName):
	m_uf( unzOpen64(strZippedFileName.c_str() ) ),
		m_cur(0)
	{
		memset(&m_gi, 0, sizeof(m_gi));

		if (m_uf)
		{
			if (UNZ_OK != unzGetGlobalInfo64( m_uf, &m_gi) )
			{
				memset(&m_gi, 0, sizeof(m_gi));
			}
		}
		return;
	}

	void UnziPackageInternal::Rewind(void)
	{
		unzGoToFirstFile( m_uf );
		m_cur = 0;

		return;
	}

	bool UnziPackageInternal::NextFileInZip(std::string& strFileName)
	{
		strFileName.erase();

		if (NULL == m_uf)
		{
			return false;
		}

		if (m_cur == m_gi.number_entry)
		{
			return false;
		}

		unz_file_info64 fileInfo;
		char fileName[512];

		if (m_cur ++ > 0)
		{
			if (UNZ_OK != unzGoToNextFile(m_uf))
			{
				return false;
			}
		}

		if (UNZ_OK != unzGetCurrentFileInfo64( m_uf, &fileInfo, fileName, sizeof(fileName), NULL, 0, NULL, 0 ))
		{
			return false;
		}

		strFileName = fileName;

		return true;
	}

	bool UnziPackageInternal::FindFileInZip(const std::string& strFileName)
	{
		using namespace std;

		if (0 == m_gi.number_entry)
		{
			return false;
		}

		if (m_cur == m_gi.number_entry)
		{
			Rewind();
		}

		const ZPOS64_T savedPos = m_cur;
		std::string strCandidate;

		do
		{
			if (!NextFileInZip(strCandidate))
			{
				break;
			}
			if (0 == strFileName.compare(strCandidate))
			{
				return true;
			}
			if (m_cur == m_gi.number_entry)
			{
				Rewind();
			}
		}while ( savedPos != m_cur );

		return false;
	}

	bool UnziPackageInternal::OpenFileInZip(void)
	{
		return UNZ_OK == unzOpenCurrentFile(m_uf);
	}

	bool UnziPackageInternal::ReadBulk(MemoryBlock& blk, size_t& validLength)
	{
		int iRead = 0;

		iRead = unzReadCurrentFile( m_uf, blk.offsetT<unsigned char>(0), static_cast<unsigned int>(blk.size()));
		if (iRead < 0)
		{
			return false;
		}
		else
		{
			validLength = (size_t)iRead;
			return UNZ_EOF != iRead;
		}
	}

	bool UnziPackageInternal::ReadBulk(MemoryBlock& blk)
	{
		unz_file_info64 fileInfo;

		if (UNZ_OK != unzGetCurrentFileInfo64( m_uf, &fileInfo, NULL, 0, NULL, 0, NULL, 0 ))
		{
			return false;
		}

		blk.alloc( static_cast<size_t>(fileInfo.uncompressed_size) );

		return unzReadCurrentFile( m_uf, blk.offsetT<unsigned char>(0), static_cast<unsigned int>(blk.size())) == fileInfo.uncompressed_size;
	}

	bool UnziPackageInternal::CloseFileInZip(void)
	{
		return UNZ_OK == unzCloseCurrentFile(m_uf);
	}

	UnziPackageInternal::~UnziPackageInternal(void)
	{
		if ( NULL != m_uf )
		{
			unzClose(m_uf);
			m_uf = NULL;
		}

		return;
	}

	UnziPackage::UnziPackage(const std::string& strZippedFileName):
	m_internal( new UnziPackageInternal(strZippedFileName) )
	{
		return;
	}

	bool UnziPackage::NextFileInZip(std::string& strFileName)
	{
		if (NULL == m_internal)
		{
			return false;
		}
		else
		{
			return m_internal->NextFileInZip(strFileName);
		}
	}

	bool UnziPackage::FindFileInZip(const std::string& strFileName)
	{
		if (NULL == m_internal)
		{
			return false;
		}
		else
		{
			return m_internal->FindFileInZip(strFileName);
		}
	}

	bool UnziPackage::OpenFileInZip(void)
	{
		if (NULL == m_internal)
		{
			return false;
		}
		else
		{
			return m_internal->OpenFileInZip();
		}
	}

	bool UnziPackage::ReadBulk(MemoryBlock& blk, size_t& validLength)
	{
		if (NULL == m_internal)
		{
			return false;
		}
		else
		{
			return m_internal->ReadBulk(blk, validLength);
		}
	}

	bool UnziPackage::ReadBulk(MemoryBlock& blk)
	{
		if (NULL == m_internal)
		{
			return false;
		}
		else
		{
			return m_internal->ReadBulk(blk);
		}
	}

	bool UnziPackage::CloseFileInZip(void)
	{
		if (NULL == m_internal)
		{
			return false;
		}
		else
		{
			return m_internal->CloseFileInZip();
		}
	}

	void UnziPackage::Rewind(void)
	{
		if (NULL != m_internal)
		{
			m_internal->Rewind();
		}

		return;
	}

	UnziPackage::~UnziPackage(void)
	{
		if (NULL != m_internal)
		{
			delete m_internal;
			m_internal = NULL;
		}
		return;
	}
=======
    static uLong GetDosDateTime(unsigned short mday, unsigned short month, unsigned short year, 
        unsigned short sec, unsigned short min, unsigned short hour)
    {
        uLong dosDateTime = 0;
        *((unsigned short*)&dosDateTime + 1) = mday | ((month + 1) << 5) | ((year - 80) << 9);
        *((unsigned short*)&dosDateTime + 0) = (sec / 2) | (min << 5) | (hour << 11);
        return dosDateTime;
    }

    class ZiPackageInternal
    {
    public:
        explicit ZiPackageInternal(const std::string& strZipName);
        bool CreateFile(const std::string& strFilePath);
        bool WriteBulk(const MemoryBlock& blk);
        bool FlushFile(void);
        bool Close(void);
        virtual ~ZiPackageInternal(void);
    private:
        zipFile m_zf;
        zip_fileinfo* m_zi;
        zip_fileinfo  m_zi_info;
    }; // class ZiPackageInternal

    ZiPackageInternal::ZiPackageInternal(const std::string& strZipName):
    m_zf( zipOpen64( strZipName.c_str(), APPEND_STATUS_CREATE) ),
        m_zi( NULL )
    {
        assert( NULL != m_zf );
        return;
    }

    bool ZiPackageInternal::CreateFile(const std::string& strFilePath)
    {
        if (NULL != m_zi)
        {
            return false;
        }

        m_zi = &m_zi_info;
        memset( m_zi, 0, sizeof(m_zi) );
        m_zi->external_fa = 32;

        time_t now;
        time(&now);
        tm* local_time = localtime(&now);

        m_zi->tmz_date.tm_sec = local_time->tm_sec;
        m_zi->tmz_date.tm_min = local_time->tm_min;
        m_zi->tmz_date.tm_hour = local_time->tm_hour;
        m_zi->tmz_date.tm_mday = local_time->tm_mday;
        m_zi->tmz_date.tm_mon = local_time->tm_mon;
        m_zi->tmz_date.tm_year = local_time->tm_year;

        // dos date
        m_zi->dosDate = GetDosDateTime( local_time->tm_mday, 
            local_time->tm_mon, 
            local_time->tm_year, 
            local_time->tm_sec, 
            local_time->tm_min, 
            local_time->tm_hour );

        return ZIP_OK == zipOpenNewFileInZip3_64( m_zf, strFilePath.c_str(), m_zi, 
            NULL, 0, NULL, 0, NULL, 
            Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0,
            -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, 
            NULL, 0, 0 );
    }

    bool ZiPackageInternal::WriteBulk(const MemoryBlock& blk)
    {
        if (NULL == m_zi)
        {
            return false;
        }

        if ( 0 != blk.size() )
        {
            return zipWriteInFileInZip( m_zf, blk.data(), static_cast<unsigned int>(blk.size()) ) >= 0;
        }
        else
        {
            return true;
        }
    }

    bool ZiPackageInternal::FlushFile(void)
    {
        if (NULL == m_zi)
        {
            return false;
        }

        bool bResult = (ZIP_OK == zipCloseFileInZip(m_zf));
        m_zi = NULL;

        return bResult;
    }

    bool ZiPackageInternal::Close(void)
    {
        assert( NULL == m_zi );

        if (NULL != m_zf)
        {
            bool bRtn = (ZIP_OK == zipClose(m_zf, NULL));
            m_zf = NULL;
            return bRtn;
        }
        else
        {
            return false;
        }
    }

    ZiPackageInternal::~ZiPackageInternal(void)
    {
        Close();
        return;
    }

    ZiPackage::ZiPackage(const std::string& strZipName):
    m_internal( new ZiPackageInternal(strZipName) )
    {
        return;
    }

    bool ZiPackage::CreateFile(const std::string& strFilePath)
    {
        if (NULL == m_internal)
        {
            return false;
        }
        else
        {
            return m_internal->CreateFile(strFilePath);
        }
    }

    bool ZiPackage::WriteBulk(const MemoryBlock& blk)
    {
        if (NULL == m_internal)
        {
            return false;
        }
        else
        {
            return m_internal->WriteBulk(blk);
        }
    }

    bool ZiPackage::FlushFile(void)
    {
        if (NULL == m_internal)
        {
            return false;
        }
        else
        {
            return m_internal->FlushFile();
        }
    }

    bool ZiPackage::Close(void)
    {
        if (NULL == m_internal)
        {
            return false;
        }
        else
        {
            return m_internal->Close();
        }
    }

    ZiPackage::~ZiPackage(void)
    {
        if (NULL != m_internal)
        {
            delete m_internal;
            m_internal = NULL;
        }

        return;
    }

    class UnziPackageInternal
    {
    public:
        UnziPackageInternal(const std::string& strZippedFileName);
        bool NextFileInZip(std::string& strFileName);
        bool FindFileInZip(const std::string& strFileName);
        bool OpenFileInZip(void);
        bool ReadBulk(MemoryBlock& blk, size_t& validLength);
        bool ReadBulk(MemoryBlock& blk);
        bool CloseFileInZip(void);
        void Rewind(void);
        virtual ~UnziPackageInternal(void);
    private:
        ZPOS64_T m_cur;
        unz_global_info64 m_gi;
        unzFile m_uf;
    }; // class UnziPackageInternal

    UnziPackageInternal::UnziPackageInternal(const std::string& strZippedFileName):
    m_uf( unzOpen64(strZippedFileName.c_str() ) ),
        m_cur(0)
    {
        memset(&m_gi, 0, sizeof(m_gi));

        if (m_uf)
        {
            if (UNZ_OK != unzGetGlobalInfo64( m_uf, &m_gi) )
            {
                memset(&m_gi, 0, sizeof(m_gi));
            }
        }
        return;
    }

    void UnziPackageInternal::Rewind(void)
    {
        unzGoToFirstFile( m_uf );
        m_cur = 0;

        return;
    }

    bool UnziPackageInternal::NextFileInZip(std::string& strFileName)
    {
        strFileName.erase();

        if (NULL == m_uf)
        {
            return false;
        }

        if (m_cur == m_gi.number_entry)
        {
            return false;
        }

        unz_file_info64 fileInfo;
        char fileName[512];

        if (m_cur ++ > 0)
        {
            if (UNZ_OK != unzGoToNextFile(m_uf))
            {
                return false;
            }
        }

        if (UNZ_OK != unzGetCurrentFileInfo64( m_uf, &fileInfo, fileName, sizeof(fileName), NULL, 0, NULL, 0 ))
        {
            return false;
        }

        strFileName = fileName;

        return true;
    }

    bool UnziPackageInternal::FindFileInZip(const std::string& strFileName)
    {
        using namespace std;

        if (0 == m_gi.number_entry)
        {
            return false;
        }

        if (m_cur == m_gi.number_entry)
        {
            Rewind();
        }

        const ZPOS64_T savedPos = m_cur;
        std::string strCandidate;

        do
        {
            if (!NextFileInZip(strCandidate))
            {
                break;
            }
            if (0 == strFileName.compare(strCandidate))
            {
                return true;
            }
            if (m_cur == m_gi.number_entry)
            {
                Rewind();
            }
        }while ( savedPos != m_cur );

        return false;
    }

    bool UnziPackageInternal::OpenFileInZip(void)
    {
        return UNZ_OK == unzOpenCurrentFile(m_uf);
    }

    bool UnziPackageInternal::ReadBulk(MemoryBlock& blk, size_t& validLength)
    {
        int iRead = 0;

        iRead = unzReadCurrentFile( m_uf, blk.data(), static_cast<unsigned int>(blk.size()));
        if (iRead < 0)
        {
            return false;
        }
        else
        {
            validLength = (size_t)iRead;
            return UNZ_EOF != iRead;
        }
    }

    bool UnziPackageInternal::ReadBulk(MemoryBlock& blk)
    {
        unz_file_info64 fileInfo;

        if (UNZ_OK != unzGetCurrentFileInfo64( m_uf, &fileInfo, NULL, 0, NULL, 0, NULL, 0 ))
        {
            return false;
        }

        blk.alloc( static_cast<size_t>(fileInfo.uncompressed_size) );

        return unzReadCurrentFile( m_uf, blk.data(), static_cast<unsigned int>(blk.size())) == fileInfo.uncompressed_size;
    }

    bool UnziPackageInternal::CloseFileInZip(void)
    {
        return UNZ_OK == unzCloseCurrentFile(m_uf);
    }

    UnziPackageInternal::~UnziPackageInternal(void)
    {
        if ( NULL != m_uf )
        {
            unzClose(m_uf);
            m_uf = NULL;
        }

        return;
    }

    UnziPackage::UnziPackage(const std::string& strZippedFileName):
    m_internal( new UnziPackageInternal(strZippedFileName) )
    {
        return;
    }

    bool UnziPackage::NextFileInZip(std::string& strFileName)
    {
        if (NULL == m_internal)
        {
            return false;
        }
        else
        {
            return m_internal->NextFileInZip(strFileName);
        }
    }

    bool UnziPackage::FindFileInZip(const std::string& strFileName)
    {
        if (NULL == m_internal)
        {
            return false;
        }
        else
        {
            return m_internal->FindFileInZip(strFileName);
        }
    }

    bool UnziPackage::OpenFileInZip(void)
    {
        if (NULL == m_internal)
        {
            return false;
        }
        else
        {
            return m_internal->OpenFileInZip();
        }
    }

    bool UnziPackage::ReadBulk(MemoryBlock& blk, size_t& validLength)
    {
        if (NULL == m_internal)
        {
            return false;
        }
        else
        {
            return m_internal->ReadBulk(blk, validLength);
        }
    }

    bool UnziPackage::ReadBulk(MemoryBlock& blk)
    {
        if (NULL == m_internal)
        {
            return false;
        }
        else
        {
            return m_internal->ReadBulk(blk);
        }
    }

    bool UnziPackage::CloseFileInZip(void)
    {
        if (NULL == m_internal)
        {
            return false;
        }
        else
        {
            return m_internal->CloseFileInZip();
        }
    }

    void UnziPackage::Rewind(void)
    {
        if (NULL != m_internal)
        {
            m_internal->Rewind();
        }

        return;
    }

    UnziPackage::~UnziPackage(void)
    {
        if (NULL != m_internal)
        {
            delete m_internal;
            m_internal = NULL;
        }
        return;
    }
>>>>>>> 337f6159b6182c037a422dc62888a41531d5c7c0

}; // namespace dj3d