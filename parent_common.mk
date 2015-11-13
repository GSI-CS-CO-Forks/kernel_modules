include $(REPO_PARENT)/common.mk

# Provide support of getting COHT version from submodules
CONFIG_SUPER_REPO = coht
CONFIG_SUPER_REPO_VERSION = $(shell cd $(REPO_PARENT); git describe --always --dirty --long --tags)
