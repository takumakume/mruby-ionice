FROM centos:latest

RUN yum install -y \
  gcc \
  make \
  bison \
  git \
  openssl-devel \
  ca-certificate \
  curl \
  ruby \
  rubygems

RUN gem install rake
