SONiC NAS ACL
===============

Access Control List for the SONiC project

Description
-----------

This repo contains the ACL portion of the Network abstraction service. This provides a programmable interface to configure system-flow entries, data-plane packet filters to NPU. 

Building
---------
Please see the instructions in the sonic-nas-manifest repo for more details on the common build tools.  [Sonic-nas-manifest](https://stash.force10networks.com/projects/SONIC/repos/sonic-nas-manifest/browse)

Build Requirements:
 - sonic-base-model
 - sonic-common
 - sonic-nas-common
 - sonic-object-library
 - sonic-logging
 - sonic-nas-ndi
 - sonic-nas-linux
 - sonic-nas-ndi-api

Dependent Packages:
  libsonic-logging-dev libsonic-logging1 libsonic-model1 libsonic-model-dev libsonic-common1 libsonic-common-dev libsonic-object-library1 libsonic-object-library-dev sonic-sai-api-dev libsonic-nas-common1 libsonic-nas-common-dev sonic-ndi-api-dev  libsonic-nas-ndi1 libsonic-nas-ndi-dev libsonic-nas-linux1 libsonic-nas-linux-dev libsonic-sai-common1 libsonic-sai-common-utils1


BUILD CMD: sonic_build  --dpkg libsonic-logging-dev libsonic-logging1 libsonic-model1 libsonic-model-dev libsonic-common1 libsonic-common-dev libsonic-object-library1 libsonic-object-library-dev sonic-sai-api-dev libsonic-nas-common1 libsonic-nas-common-dev sonic-ndi-api-dev  libsonic-nas-ndi1 libsonic-nas-ndi-dev libsonic-nas-linux1 libsonic-nas-linux-dev --apt libsonic-sai-common1 libsonic-sai-common-utils1 -- clean binary

(c) Dell 2016

