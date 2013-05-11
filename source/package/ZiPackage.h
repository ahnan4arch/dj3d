#pragma once

#include <string>
#include "utility/utility.h"

namespace dj3d
{
    class ZiPackageInternal;

    class ZiPackage
    {
    private:
        ZiPackage( const ZiPackage& );
        ZiPackage& operator = ( const ZiPackage& );
    public:
        explicit ZiPackage(const std::string& strZipName);
        bool CreateFile(const std::string& strFilePath);
        bool WriteBulk(const MemoryBlock& blk);
        bool FlushFile(void);
        bool Close(void);
        virtual ~ZiPackage(void);
    private:
        ZiPackageInternal * m_internal;

    }; // class ZiPackage

    class UnziPackageInternal;

    class UnziPackage
    {
    private:
        UnziPackage( const UnziPackage& );
        UnziPackage& operator = ( const UnziPackage& );
    public:
        UnziPackage(const std::string& strZippedFileName);
        bool NextFileInZip(std::string& strFileName);
        bool FindFileInZip(const std::string& strFileName);
        bool OpenFileInZip(void);
        bool ReadBulk(MemoryBlock& blk, size_t& validLength);
<<<<<<< HEAD
		bool ReadBulk(MemoryBlock& blk);
=======
        bool ReadBulk(MemoryBlock& blk );
>>>>>>> 337f6159b6182c037a422dc62888a41531d5c7c0
        bool CloseFileInZip(void);
        void Rewind(void);
        virtual ~UnziPackage(void);
    private:
        UnziPackageInternal* m_internal;
    };

}; // namespace dj3d
