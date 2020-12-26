#!/bin/sh

usage()
{
    echo "usage: $0 [package]"
    exit 1
}

smart_install()
{
    if [ $# -ne 1 ]; then
        usage
    fi

    PACKAGE_NAME="$1"
    DUMMY=$(dpkg -l | grep -E "^.*$PACKAGE_NAME.*$")
    RESULT=$(echo $?)

    if [ $RESULT -ne 0 ]; then
        while true
        do
            read -p "preparing to install package: $PACKAGE_NAME, [yes/no]: " prompt
            case $prompt in
            y|yes)
                sudo apt-get -y install "$PACKAGE_NAME"
                break;;
            n|no)
                break;;
            *)
            echo "error choice: " $prompt;;
            esac
        done
    else
        echo "$PACKAGE_NAME has already installed."
    fi
}

smart_install liblog4cxx10-dev
smart_install libboost-thread-dev
smart_install libboost-filesystem-dev
smart_install libboost-system-dev
smart_install libboost-program-options-dev
smart_install libboost-chrono-dev
smart_install libboost-iostreams-dev
smart_install libboost-locale-dev
