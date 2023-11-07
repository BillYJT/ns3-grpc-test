check_include_file_cxx(stdint.h HAVE_STDINT_H)

set (link_libs ${libcore})

# grpc
# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(Threads REQUIRED)

option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
message("gRPC_FOUND: " ${gRPC_FOUND})
message("SYSREPO_INCLUDE_DIRS: " ${SYSREPO_INCLUDE_DIRS})
message("gRPC_Dir: " ${gRPC_DIR})

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
get_filename_component(ns3_proto "${CMAKE_CURRENT_SOURCE_DIR}/model/helloworld.proto" ABSOLUTE)
get_filename_component(ns3_proto_path "${ns3_proto}" PATH)

# Generated sources
get_filename_component(proto_gen_dir "${CMAKE_CURRENT_SOURCE_DIR}/model/proto_generated_file/" ABSOLUTE)
set(ns3_proto_srcs "${proto_gen_dir}/helloworld.pb.cc")
set(ns3_proto_hdrs "${proto_gen_dir}/helloworld.pb.h")
set(ns3_grpc_srcs "${proto_gen_dir}/helloworld.grpc.pb.cc")
set(ns3_grpc_hdrs "${proto_gen_dir}/helloworld.grpc.pb.h")
add_custom_command(
      OUTPUT "${ns3_proto_srcs}" "${ns3_proto_hdrs}" "${ns3_grpc_srcs}" "${ns3_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${proto_gen_dir}"
        --cpp_out "${proto_gen_dir}"
        -I "${ns3_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${ns3_proto}"
      DEPENDS "${ns3_proto}")
# ns3_grpc_proto
add_library(ns3_grpc_proto
  ${ns3_grpc_srcs}
  ${ns3_grpc_hdrs}
  ${ns3_proto_srcs}
  ${ns3_proto_hdrs})
target_link_libraries(ns3_grpc_proto PUBLIC
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
set_target_properties(absl::flags PROPERTIES IMPORTED_GLOBAL TRUE)
set_target_properties(absl::flags_parse PROPERTIES IMPORTED_GLOBAL TRUE)
set_target_properties(${_REFLECTION} PROPERTIES IMPORTED_GLOBAL TRUE)
set_target_properties(${_GRPC_GRPCPP} PROPERTIES IMPORTED_GLOBAL TRUE)
set_target_properties(${_PROTOBUF_LIBPROTOBUF} PROPERTIES IMPORTED_GLOBAL TRUE)
install(
  TARGETS ns3_grpc_proto
  # TARGETS zlibstatic grpc grpc++ grpc++_reflection address_sorting upb gpr ssl crypto ns3_grpc_proto
  EXPORT ns3ExportTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/
  RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}/
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ns3"
  PRIVATE_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ns3"
)


#grpc libs
list (APPEND link_libs  
                        ns3_grpc_proto
                        absl::flags
                        absl::flags_parse
                        ${_REFLECTION}
                        ${_GRPC_GRPCPP}
                        ${_PROTOBUF_LIBPROTOBUF}
                        )
 

if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources    
        #test/grpc-test-examples-test-suite.cc
        )
endif()    

build_lib(
    LIBNAME grpc-test
    SOURCE_FILES model/grpc-test.cc
                 helper/grpc-test-helper.cc 
    HEADER_FILES model/grpc-test.h
                 helper/grpc-test-helper.h
    LIBRARIES_TO_LINK ${link_libs} 
    TEST_SOURCES test/grpc-test-test-suite.cc
                 ${examples_as_tests_sources}
)
    
