#!/bin/bash
DYLD_FRAMEWORK_PATH=/Users/juliakasperek/Qt/6.9.3/macos/lib${DYLD_FRAMEWORK_PATH:+:$DYLD_FRAMEWORK_PATH}
export DYLD_FRAMEWORK_PATH
QT_PLUGIN_PATH=/Users/juliakasperek/Qt/6.9.3/macos/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}
export QT_PLUGIN_PATH
exec "$@"
