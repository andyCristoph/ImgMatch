#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <stdexcept>


#ifdef ENABLE_LOG

#define INIT_LOG(logDir, fileNamePrefix, fileNameExt) \
    InitLog(logDir, fileNamePrefix, fileNameExt)

#define LOG( text ) \
    do { \
        std::stringstream sstr; \
        sstr << Logger::GetCurrentDateTimeStr() << " " << __FILE__ << ":" \
             << __FUNCTION__ << "(): " << text; \
        Logger::Log( sstr.str() ); \
    } while( false )

#define THROW( text ) \
    do { \
        std::stringstream sstr; \
        sstr << Logger::GetCurrentDateTimeStr() << " ***EXCEPTION*** " \
             << __FILE__ << ":" << __LINE__ << ": " << text; \
        Logger::Log( sstr.str() ); \
        throw std::runtime_error( sstr.str() ); \
    } while( false )

#else  // No logging

#define INIT_LOG(logDir, fileNamePrefix, fileNameExt)

#define LOG( text )

#define THROW( text ) \
    do { \
        std::stringstream sstr; \
        sstr << Logger::GetCurrentDateTimeStr() << " ***EXCEPTION*** " \
             << __FILE__ << ":" << __LINE__ << ": " << text; \
        throw std::runtime_error( sstr.str() ); \
    } while( false )

#endif // ENABLE_LOG


class Logger
{
  public:
    ~Logger();

    static void Log( const std::string& text );

    static void InitLog( const std::string& logDir, 
                         const std::string& fileNamePrefix,
                         const std::string& fileNameExt );

    static std::string GetCurrentDateTimeStr( const char* hour=":", 
                                              const char* min=":", 
                                              const char* sec="" );

  private:
    Logger( const std::string& logDir="./", 
            const std::string& fileNamePrefix="log_",
            const std::string& fileNameExt=".txt" );

    static std::ofstream& GetLogFile();

  private:
//  static Logger* sLogger;
    static std::auto_ptr<Logger> sLogger;

    std::string   mLogFileName;
    std::ofstream mLogFile;
};


#endif /* __LOGGER_H__ */
