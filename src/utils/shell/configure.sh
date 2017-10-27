#!/usr/bin/env bash

set -u # not allow use undeclared variables
set -e # exit when a command fails

WHITE=$(tput setaf 15)
RED=$(tput setaf 9)
YELLOW=$(tput setaf 11)
GREEN=$(tput setaf 2)
PURPLE=$(tput setaf 5)
BLUE=$(tput setaf 12)
BOLD=$(tput bold)
ERROR="${BOLD}${RED}"
NORMAL=$(tput sgr0)

DISTRO=`lsb_release -si`
VERSION=`lsb_release -sr`
CORES=`nproc`
ARCH=`uname -m`

SUDO_UPDATE="sudo apt-get update"
SUDO_INSTALL="sudo apt-get install"

ERROR_OCURRED=0

ASK_INSTALL=1
INSTALL_ESPECIFIC=0

SUDO_UPDATE () {
	sudo apt-get update
}

# INSTALL A PROGRAM; $1: program detail $2: program name
INSTALL () {

	# get number of parameters
	if [ "$#" -eq 2 ]; then
		echo "${BOLD}${BLUE}Installing $1... ${NORMAL}"
		CMD="sudo apt-get -y install $2"

		if ${CMD}; then
			echo "${BOLD}${GREEN}[OK...]${NORMAL}"
		else
			echo "${ERROR}[ERROR...]${WHITE}"
			ERROR_OCURRED=1
		fi
	else
		echo "${ERROR}INCORRECT NUMBER OF PARAMETERS ${NORMAL}"
		ERROR_OCURRED=1
	fi

}

# CREATES TEMP DIRECTORY
CREATE_WORKSPACE () {
	DIR=`pwd`
	WORK_DIR=`mktemp -d -p ${DIR}`
	cd ${WORK_DIR}
}

EXIT () {
	cd ..
	rm -rf "$WORK_DIR"
}

# VERIFY IF ITS RUNNING AS ROOT
if [ "$EUID" -ne 0 ]; then 
	echo "Please run as root"
	exit
fi

# PARSE PARAMETERS
while getopts :y OPCAO; do
	case "${OPCAO}" in
    y)	
	    ASK_INSTALL=0 
	;;
		
	\?)
	    echo "INVALID PARAMETER"
	    exit
	;;
 	esac
done

# GET INFORMATIONS FROM THE SYSTEM
GET_INFORMATION () {
	echo "${BOLD}${BLUE}Checking system information...";
	sleep 1
	if [[ "${DISTRO}" = "Ubuntu" || "$DISTRO" = "Debian" ]]; then
		echo "${GREEN}SYSTEM DETECTED: ${WHITE}${DISTRO} ${VERSION}"
	else
		echo "${ERROR}ERROR. There's no support to your distro."
	fi
	sleep 1
}

# INSTALL GENERAL DEPENDENCIES
INSTALL_COMMONS_DEPEND () {

	ok=1

	if [ ${ASK_INSTALL} -eq 1 ]; then
		echo "${WHITE}${BOLD}DO YOU WANT TO INSTALL DEPENDENCIES ? (Y\n) ${NORMAL}"
		read answer
		if [ "${answer}" = "n" ]; then
			ok=0
		fi
	fi

	if [ ${ok} -eq 1 ]; then

		INSTALL Git git
		INSTALL g++ g++
		INSTALL webcam-control v4l-utils
		INSTALL cmake cmake
		INSTALL qt5 qt5-default
		INSTALL libxvidcore libxvidcore-dev
		INSTALL libv4l libv4l-dev
		INSTALL libxine2 libxine2-dev
		INSTALL arduino arduino-mk
		INSTALL arduino arduino
		INSTALL glut freeglut3-dev
		INSTALL zip zip

		if [ ${ERROR_OCURRED} -eq "1" ]; then
			echo "${ERROR}SOME ERROR OCURRED INSTALLING COMMONS DEPENDENCIES. FIX THEM AND EXECUTE AGAIN! ${WHITE}"
			EXIT
		else
			echo "${BOLD}${GREEN}COMMONS DEPENDENCIES INSTALLED SUCCESFULLY${WHITE}"
		fi

		sleep 3
	fi
}

# INSTALL GUV 
INSTALL_GUV () {

	ok=1

	if [ ${ASK_INSTALL} -eq 1 ]; then
		echo "${WHITE}${BOLD}DO YOU WANT TO INSTALL GUV ? (Y\n) ${NORMAL}"
		read answer
		if [ "${answer}" = "n" ]; then
			ok=0
		fi
	fi

	if [ ${ok} -eq 1 ]; then

		#UBUNTU
		if [[ ${DISTRO} == "Ubuntu" || $DISTRO == "neon" ]]; then
			echo "${GREEN}${BOLD}INSTALLING GUV VIEW FOR UBUNTU${NORMAL}"
			sudo add-apt-repository -y ppa:pj-assis/testing
			SUDO_UPDATE
			INSTALL guvcview guvcview

		#DEBIAN 64 bits
		elif [[ ${DISTRO} == "Debian" && ${ARCH} == "x86_64" ]]; then
			echo "${GREEN}${BOLD}INSTALLING GUV VIEW FOR DEBIAN${NORMAL}"
			wget http://nbtelecom.dl.sourceforge.net/project/guvcview/debian-bin/guvcview_2.0.2%2Bubuntu1_ppa1_amd64.deb -O guvcview.deb
			wget http://nbtelecom.dl.sourceforge.net/project/guvcview/debian-bin/libguvcview-1.1-0_2.0.2%2Bubuntu1_ppa1_amd64.deb -O libguvcview.deb
			if sudo dpkg -i libguvcview.deb; then
				sudo apt-get install -f
				if sudo dpkg -i guvcview.deb; then
					echo "${GREEN}${BOLD}GUVCVIEW WAS INSTALLED ${NORMAL}"
				else
					echo "${ERROR}ERROR INSTALLING GUVCVIEW${NORMAL}"
					ERROR_OCURRED=1
				fi
			else
				echo "${ERROR}ERROR INSTALLING LIBGUVCVIEW${NORMAL}"
				ERROR_OCURRED=1
			fi
		else
			echo "${ERROR}THERE IS NO SUPPORT TO YOUR DISTRO${NORMAL}"
			ERROR_OCURRED=1
		fi

		if [ ${ERROR_OCURRED} -eq 1 ]; then
			echo "${ERROR}SOME ERROR OCURRED. FIX AND EXECUTE AGAIN!${NORMAL}"
		fi
	fi
}

INSTALL_OPENCV () {

	ok=1

	if [ ${ASK_INSTALL} -eq 1 ]; then
		echo "${WHITE}${BOLD}DO YOU WANT TO INSTALL OPENCV ? (Y\n) ${NORMAL}"
		read answer
		if [ "${answer}" = "n" ]; then
			ok=0
		fi
	fi

	if [ ${ok} -eq 1 ]; then

		echo "${GREEN}${BOLD}INSTALLING OPENCV 3.0${NORMAL}"

		EXIST=`find -maxdepth 1 -type d -name 'opencv-3.0.0'`
		if [ -z "${EXIST}" ]; then
			echo "${GREEN}${BOLD}DOWNLOADING OPENCV 3.0${NORMAL}"
			wget https://codeload.github.com/opencv/opencv/zip/3.0.0 -O opencv.zip
			unzip opencv.zip
		fi

		cd opencv-3.0.0/
		mkdir build && cd build
		cmake -DENABLE_PRECOMPILED_HEADERS=OFF -DWITH_QT=ON -DWITH_OPENGL=ON -DFORCE_VTK=ON -DWITH_TBB=ON -DWITH_GDAL=ON -DWITH_XINE=ON DBUILD_EXAMPLES=ON -D WITH_IPP=OFF ..
		make -j${CORES}
		sudo make install
		sudo ldconfig
		cd ../../
		echo "${BOLD}${GREEN}OPENCV IS NOW INSTALLED ${NORMAL}"

	fi
}

INSTALL_GTKMM () {
	ok=1

	if [ ${ASK_INSTALL} -eq 1 ]; then
		echo "${WHITE}${BOLD}DO YOU WANT TO INSTALL GTKMM ? (Y\n) ${NORMAL}"
		read answer
		if [ "${answer}" = "n" ]; then
			ok=0
		fi
	fi

	if [ ${ok} -eq 1 ]; then

		if [[ ${DISTRO} = "Ubuntu" ]]; then
			echo "${GREEN}${BOLD}INSTALLING GTKMM FOR UBUNTU...${NORMAL}"
			INSTALL gtkmm libgtkmm-3.0-dev
			INSTALL gtkmm libgstreamermm-1.0-dev
			INSTALL gtkmm libgtkmm-3.0-doc
			INSTALL gtkmm libgstreamermm-1.0-doc
			INSTALL gtkmm devhelp

		else
			echo "${ERROR}THERE'S NO SUPPORT TO YOUR DISTRO${NORMAL}"
			ERROR_OCURRED=1
		fi
	fi
}

INSTALL_XCTU () {
	ok=1

	if [ ${ASK_INSTALL} -eq 1 ]; then
		echo "${WHITE}${BOLD}DO YOU WANT TO INSTALL XCTU ? (Y\n) ${NORMAL}"
		read answer
		if [ "${answer}" == "n" ]; then
			ok=0
		fi
	fi

	if [ ${ok} -eq 1 ]; then
		wget http://ftp1.digi.com/support/utilities/40002881_D.run -O xctu
		chmod +x xctu
		./xctu
		echo "sudo /opt/Digi/XCTU-NG/app" >> /usr/bin/xctu
		chmod +x /usr/bin/xctu
	fi
}

INSTALL_XBEE_H () {
	ok=1

	if [ ${ASK_INSTALL} -eq 1 ]; then
		echo "${WHITE}${BOLD}DO YOU WANT TO INSTALL XBEE_H_ ? (Y\n) ${NORMAL}"
		read answer
		if [ "${answer}" == "n" ]; then
			ok=0
		fi
	fi
	
	if [ ${ok} -eq 1 ]; then
		git clone https://github.com/andrewrapp/xbee-arduino.git
		mv xbee-arduino/ /usr/share/arduino/libraries/xbee
	fi
}

CREATE_WORKSPACE
SUDO_UPDATE

GET_INFORMATION

INSTALL_COMMONS_DEPEND
INSTALL_XBEE_H
INSTALL_GUV
INSTALL_OPENCV
INSTALL_GTKMM
INSTALL_XCTU

if [ $ERROR_OCURRED -eq 1 ]; then
	echo "${RED}${BOLD}SOME ERRORS OCURRED${NORMAL}"
fi

EXIT
