sonic-nas-acl
===============

This repo contains the access-control List (ACL) portion of the network abstraction service (NAS). This provides a programmable interface to configure system-flow entries, and data-plane packet filters to NPU. 

Build
---------
See [sonic-nas-manifest](https://github.com/Azure/sonic-nas-manifest) for complete information on common build tools.

### Build requirements
* `sonic-base-model`
* `sonic-common`
* `sonic-nas-common`
* `sonic-object-library`
* `sonic-logging`
* `sonic-nas-ndi`
* `sonic-nas-linux`
* `sonic-nas-ndi-api`

### Dependent packages
* `libsonic-logging-dev` 
* `libsonic-logging1` 
* `libsonic-model1` 
* `libsonic-model-dev` 
* `libsonic-common1` 
* `libsonic-common-dev` 
* `libsonic-object-library1` 
* `libsonic-object-library-dev` 
* `sonic-sai-api-dev` 
* `libsonic-nas-common1` 
* `libsonic-nas-common-dev` 
* `sonic-ndi-api-dev`  
* `libsonic-nas-ndi1` 
* `libsonic-nas-ndi-dev` 
* `libsonic-nas-linux1` 
* `libsonic-nas-linux-dev` 
* `libsonic-sai-common1` 
* `libsonic-sai-common-utils1`

### Build command
    sonic_build  --dpkg libsonic-logging-dev libsonic-logging1 libsonic-model1 libsonic-model-dev libsonic-common1 libsonic-common-dev libsonic-object-library1 libsonic-object-library-dev sonic-sai-api-dev libsonic-nas-common1 libsonic-nas-common-dev sonic-ndi-api-dev  libsonic-nas-ndi1 libsonic-nas-ndi-dev libsonic-nas-linux1 libsonic-nas-linux-dev --apt libsonic-sai-common1 libsonic-sai-common-utils1 -- clean binary

(c) Dell 2016
