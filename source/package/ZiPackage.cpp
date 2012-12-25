#include "zip.h"
#include "unzip.h"
#include "ZiPackage.h"
#include <assert.h>

namespace dj3d
{
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
    m_zf( zipOpen2_64( strZipName.c_str(), 0, NULL, NULL ) ),
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

        m_zi = NULL;

        return ZIP_OK == zipCloseFileInZip(m_zf);
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

}; // namespace dj3d