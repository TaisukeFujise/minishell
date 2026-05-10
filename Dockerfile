FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    gcc \
    make \
    libreadline-dev \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /minishell

CMD ["/bin/bash"]
