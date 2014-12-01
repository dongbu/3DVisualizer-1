#ifndef LOGGER_H
#define LOGGER_H

#include "singleton.h"

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief The Logger class handles the application's log.
 *
 * This class is responsible for storing any log messages issued. The log itself
 * may be directed to a file or another output stream.
 *
 * There are also severity levels associated with each log message. These levels
 * are stored in the log history.
 *
 * To use this logger, the user must first specify the log stream, e.g.
 * int main(int, char**)
 * {
 *   Logger::getInstance()->setLogStream(&std::cout);
 *   ...
 * }
 *
 * The above example will output all of the logger's messages to the terminal.
 * The stream may also be defined during the course of execution, for instance,
 * the developer may want to log the results of one function in a different
 * file, this is done by calling the setLogStream before calling the
 * aforementioned function.
 */
class Logger : public Singleton<Logger>
{
public:
  void log(std::string);
  void warn(std::string);
  void error(std::string);
  void setLogStream(std::ostream*);

private:
  friend class Singleton<Logger>;
  std::ostream* m_pStream;
  std::vector<std::string> m_vLog;

  Logger() {m_pStream = NULL;}
  ~Logger() {m_pStream = NULL;}
};

#endif // LOGGER_H
