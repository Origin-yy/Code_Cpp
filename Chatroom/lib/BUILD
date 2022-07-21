cc_library(
    name = "TaskQueue",
    srcs = ["TaskQueue.cc"],
    hdrs = ["TaskQueue.hpp"],
)

cc_library(
    name = "ThreadPool",
    srcs = ["ThreadPool.cc"],
    hdrs = ["ThreadPool.hpp"],
    deps = [
        ":TaskQueue",
    ],
)

cc_binary(
    name = "test",
    srcs = ["test.cc"],
    deps = [
        ":ThreadPool",
    ],
)