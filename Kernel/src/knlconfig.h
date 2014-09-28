#ifndef KNLCONFIG_H
#define KNLCONFIG_H

#ifdef __GNUC__
  #define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
  #define DEPRECATED __declspec(deprecated)
#else
  #pragma message("WARNING: You need to implement the DEPRECATED macro for this compiler")
  #define DEPRECATED
#endif

#include "dataset.h"
#include "tfunction.h"
#include "logger.h"
#include "datahandler.h"
#include "datatransfer.h"
#include "datasetmanager.h"
#include "tfmanager.h"

#endif // KNLCONFIG_H
