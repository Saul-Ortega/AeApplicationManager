TEMPLATE = subdirs

SUBDIRS += \
    application-core \
    application-desktop

application-desktop.depends = application-core
