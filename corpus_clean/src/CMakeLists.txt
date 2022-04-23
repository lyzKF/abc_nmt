aux_source_directory("./IniConfig" Ini_SRCS)
add_library(iniconfig SHARED ${Ini_SRCS})

aux_source_directory("./ThreadPool" Thread_SRCS)
add_library(threadpool SHARED ${Thread_SRCS})

aux_source_directory("./ProcessCorpus" Process_SRCS)
add_library(processcorpus SHARED ${Process_SRCS})