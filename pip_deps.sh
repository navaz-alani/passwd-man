#! /bin/bash

# By default, installation will be performed using
#   pip3. Change this to the pip that you want to use.
PIP="pip3"

# This will install pman's Python dependency
install_req() {
    pip_test=("$PIP" --version)

    if [[ "$?" != "0" ]]; then
        echo "ERROR [REQ]: $PIP is not installed!"
    else
        "$PIP" install pyperclip
    fi
}

install_req
