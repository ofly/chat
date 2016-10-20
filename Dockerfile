# version: 0.0.1
FROM centos
MAINTAINER ofly "hi_fly@yeah.net"
ADD server /root/server
ENTRYPOINT ["/root/server"]
EXPOSE 4567
