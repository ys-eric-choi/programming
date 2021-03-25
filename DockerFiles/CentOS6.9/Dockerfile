#ARG BASE_TAG
FROM centos:centos6.9

# Update yum
RUN mkdir /var/cache/yum/x86_64
RUN mkdir /var/cache/yum/x86_64/6
RUN mkdir /var/cache/yum/x86_64/6/base
RUN mkdir /var/cache/yum/x86_64/6/extras
RUN mkdir /var/cache/yum/x86_64/6/updates

RUN echo "https://vault.centos.org/6.9/os/x86_64/" > /var/cache/yum/x86_64/6/base/mirrorlist.txt
RUN echo "http://vault.centos.org/6.9/extras/x86_64/" > /var/cache/yum/x86_64/6/extras/mirrorlist.txt
RUN echo "http://vault.centos.org/6.9/updates/x86_64/" > /var/cache/yum/x86_64/6/updates/mirrorlist.txt

RUN yum -y upgrade

# Install package
RUN yum install -y gcc
RUN yum install -y gcc-c++
RUN yum install -y git
RUN yum install -y gdb
RUN yum install -y valgrind
RUN yum install -y wget
RUN yum install -y cmake
RUN yum install -y java
RUN yum install -y java-1.8.0-openjdk-devel.x86_64
RUN yum install -y vim

RUN yum install -y httpd
RUN yum install -y mysql
RUN yum install -y mysql-server

RUN yum install -y sudo
RUN yum install -y man

# Install python3
RUN yum install -y RUN yum-utils
RUN yum groupinstall -y development

RUN yum install -y epel-release.noarch
RUN yum install -y https://repo.ius.io/ius-release-el6.rpm

RUN yum install -y python34u python34u-libs python34u-devel 
RUN yum install -y python34-pip

# Clear cache
RUN yum clean all
RUN rm -rf /tmp/*
RUN rm -rf /var/cache/yum
