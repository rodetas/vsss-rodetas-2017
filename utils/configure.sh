#!/bin/bash

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
ARCH=`uname -m`

SUDO_UPDATE="sudo apt-get update"
SUDO_INSTALL="sudo apt-get install"

ERROR_OCURRED=0

ASK_INSTALL=1
INSTALL_ESPECIFIC=0

GET_INFORMATION () {
	echo "${BOLD}${BLUE}Checking system information...";
	sleep 1;
	if [[ $DISTRO == "Ubuntu" || $DISTRO == "Debian"  || $DISTRO == "neon" ]]; then
		echo "${GREEN}SYSTEM DETECTED: ${WHITE}${DISTRO} ${VERSION}";
	else
		echo "${ERROR}ERROR. There is no support to your distro.";
	fi
	sleep 1;
}

SUDO_UPDATE () {
	sudo apt-get update;
}

CLONE_PROJECT () {
	echo "${GREEN}${BOLD}CLONING RODETAS SOURCE CODE${NORMAL}";
	git clone https://manoeel@bitbucket.org/manoeel/rodetas.git;
}

INSTALL_GUV () {

	ok=1;

	if [ ${ASK_INSTALL} -eq 1 ]; then
		echo "${WHITE}${BOLD}DO YOU WANT TO INSTALL GUV ? (Y\n) ${NORMAL}";
		read answer;
		if [ "${answer}" == "n" ]; then
			ok=0;
		fi
	fi

	if [ ${ok} -eq 1 ]; then

		#UBUNTU
		if [[ ${DISTRO} == "Ubuntu" || $DISTRO == "neon" ]]; then
			echo "${GREEN}${BOLD}INSTALLING GUV VIEW FOR UBUNTU${NORMAL}";
			sudo add-apt-repository -y ppa:pj-assis/testing;
			SUDO_UPDATE;
			INSTALL guv-view guvcview;

		#DEBIAN 64 bits
		elif [[ ${DISTRO} == "Debian" && ${ARCH} == "x86_64" ]]; then
			echo "${GREEN}${BOLD}INSTALLING GUV VIEW FOR DEBIAN${NORMAL}";
			wget http://nbtelecom.dl.sourceforge.net/project/guvcview/debian-bin/guvcview_2.0.2%2Bubuntu1_ppa1_amd64.deb -O guvcview.deb;
			wget http://nbtelecom.dl.sourceforge.net/project/guvcview/debian-bin/libguvcview-1.1-0_2.0.2%2Bubuntu1_ppa1_amd64.deb -O libguvcview.deb;
			if sudo dpkg -i libguvcview.deb; then
				sudo apt-get install -f;
				if sudo dpkg -i guvcview.deb; then
					echo "${GREEN}${BOLD}GUVCVIEW WAS INSTALLED ${NORMAL}";
				else
					echo "${ERROR}ERROR INSTALLING GUVCVIEW${NORMAL}";
					ERROR_OCURRED=1;
				fi
			else
				echo "${ERROR}ERROR INSTALLING LIBGUVCVIEW${NORMAL}";
				ERROR_OCURRED=1;
			fi
		else
			echo "${ERROR}THERE IS NO SUPPORT TO YOUR DISTRO${NORMAL}";
			ERROR_OCURRED=1;
		fi

		if [ ${ERROR_OCURRED} -eq 1 ]; then
			echo "${ERROR}SOME ERROR OCURRED. FIX AND EXECUTE AGAIN!${NORMAL}";
		fi
	fi
}

INSTALL_OPENCV () {

	ok=1;

	if [ ${ASK_INSTALL} ]; then
		echo "${WHITE}${BOLD}DO YOU WANT TO INSTALL OPENCV ? (Y\n) ${NORMAL}";
		read answer;
		if [ "${answer}" == "n" ]; then
			ok=0;
		fi
	fi

	if [ ${ok} -eq 1 ]; then

		echo "${GREEN}${BOLD}INSTALLING OPENCV 3.0${NORMAL}";

		EXIST=`find -maxdepth 1 -type d -name 'opencv-3.0.0'`
		if [ -z "${EXIST}" ]; then
			echo "${GREEN}${BOLD}DOWNLOADING OPENCV 3.0${NORMAL}";
			wget https://codeload.github.com/opencv/opencv/zip/3.0.0 -O opencv.zip;
			unzip opencv.zip;
		fi

		cd opencv-3.0.0/;
		mkdir build && cd build;
		cmake -DWITH_QT=ON -DWITH_OPENGL=ON -DFORCE_VTK=ON -DWITH_TBB=ON -DWITH_GDAL=ON -DWITH_XINE=ON DBUILD_EXAMPLES=ON -D WITH_IPP=OFF ..;
		make -j4;
		sudo make install;
		sudo ldconfig;
		cd ../../;
		echo "${BOLD}${GREEN}OPENCV IS NOW INSTALLED ${NORMAL}";

	fi
}

INSTALL_SFML () {

	ok=1;

	if [ ${ASK_INSTALL} -eq 1 ]; then
		echo "${WHITE}${BOLD}DO YOU WANT TO INSTALL SFML ? (Y\n) ${NORMAL}";
		read answer;
		if [ "${answer}" == "n" ]; then
			ok=0;
		fi
	fi

	if [ ${ok} -eq 1 ]; then

		if [[ ${DISTRO} == "Ubuntu" || ${DISTRO} == "neon" ]]; then
			echo "${GREEN}${BOLD}INSTALLING SFML FOR UBUNTU${NORMAL}";
			INSTALL SFML libsfml-dev;

		elif [ ${DISTRO} == "Debian" ]; then
			echo "${GREEN}${BOLD}INSTALLING SFML FOR DEBIAN${NORMAL}";
			INSTALL sfml-dependencies libpthread-stubs0-dev;
			INSTALL sfml-dependencies libgl1-mesa-dev;
			INSTALL sfml-dependencies libx11-dev;
			INSTALL sfml-dependencies libxrandr-dev;
			INSTALL sfml-dependencies libfreetype6-dev;
			INSTALL sfml-dependencies libglew1.5-dev;
			INSTALL sfml-dependencies libsndfile1-dev;
			INSTALL sfml-dependencies libopenal-dev;
			INSTALL sfml-dependencies libxcb-image0;
			INSTALL sfml-dependencies libudev-dev;
			INSTALL sfml-dependencies libgudev-1.0-0;
			INSTALL sfml-dependencies libxcb-image0-dev;
			INSTALL sfml-dependencies libjpeg-dev;

			EXIST=`find -maxdepth 1 -name 'sfml'`;

			# file were not downloaded
			if [ -z "${EXIST}" ]; then
				echo "${BOLD}{$GREEN}DOWNLOADING SFML...${NORMAL}";
				wget http://mirror3.sfml-dev.org/files/SFML-2.4.0-sources.zip -O sfml.zip;
				unzip sfml;
			fi

			sudo ldconfig;
			cd SFML-2.4.0;
			cmake .;
			make -j4;
			sudo make install;
			sudo ldconfig;
			cd ..

		fi
	fi
}

INSTALL_TGUI () {

	ok=1;

	if [ ${ASK_INSTALL} -eq 1 ]; then
		echo "${WHITE}${BOLD}DO YOU WANT TO INSTALL TGUI ? (Y\n) ${NORMAL}";
		read answer;
		if [ "${answer}" == "n" ]; then
			ok=0;
		fi
	fi

	if [ ${ok} -eq 1 ]; then

		if [[ "${DISTRO}" == "Ubuntu" || "$DISTRO" == "neon" ]]; then
			echo "${GREEN}${BOLD}INSTALLING TGUI FOR UBUNTU${NORMAL}";
			sudo add-apt-repository -y ppa:texus/tgui
			SUDO_UPDATE
			INSTALL TGUI libtgui-dev
		elif [ "${DISTRO}" == "Debian" ]; then
			echo "${GREEN}${BOLD}INSTALLING TGUI FOR DEBIAN${NORMAL}";
			EXIST=`find -maxdepth 1 -name 'TGUI-0.7.1'`

			if [ -z "${EXIST}" ]; then
				echo "${GREEN}${BOLD}DOWNLOADING TGUI${NORMAL}";
				wget https://codeload.github.com/texus/TGUI/zip/0.7.1 -O tgui;
				unzip tgui;
			fi

			cd TGUI-0.7.1/;
			cmake .;
			sudo make install;
			sudo ldconfig;
		fi
	fi
}

INSTALL () {

	# get number of parameters
	if [ "$#" -eq 2 ]; then
		echo "${BOLD}${BLUE}Installing $1... ${NORMAL}";
		CMD="sudo apt-get -y install $2";

		if ${CMD}; then
			echo "${BOLD}${GREEN}[OK...]${NORMAL}";
		else
			echo "${ERROR}[ERROR...]${WHITE}";
			ERROR_OCURRED=1;
		fi
	else
		echo "${ERROR}INCORRECT NUMBER OF PARAMETERS ${NORMAL}";
		ERROR_OCURRED=1;
	fi

}

INSTALL_COMMONS_DEPEND () {

	ok=1;

	if [ ${ASK_INSTALL}  -eq 1 ]; then
		echo "${WHITE}${BOLD}DO YOU WANT TO INSTALL DEPENDENCIES ? (Y\n) ${NORMAL}";
		read answer;
		if [ "${answer}" == "n" ]; then
			ok=0;
		fi
	fi

	if [ ${ok} -eq 1 ]; then

		INSTALL Git git;
		INSTALL g++ g++;
		INSTALL webcam-control uvcdynctrl;
		INSTALL cmake cmake;
		INSTALL qt5 qt5-default;
		INSTALL libxvidcore libxvidcore-dev;
		INSTALL libv4l libv4l-dev;
		INSTALL libxine2 libxine2-dev;
		INSTALL arduino arduino-mk;
		INSTALL arduino arduino;
		INSTALL glut freeglut3-dev;
		INSTALL zip zip;

		if [ ${ERROR_OCURRED} -eq "1" ]; then
			echo "${ERROR}SOME ERROR OCURRED. FIX THEM AND EXECUTE AGAIN! ${WHITE}";
			exit 1;
		else
			echo "${BOLD}${GREEN}COMMONS DEPENDENCIES INSTALLED ${WHITE}";
		fi

		sleep 3;

	fi
}

CREATE_WORKSPACE () {
	mkdir installation;
	cd installation;
}

if [ "$EUID" -ne 0 ]; then 
		echo "Please run as root"
		exit
fi

while getopts :y OPCAO; do
	case "${OPCAO}" in
    	y)	
	    ASK_INSTALL=0 
	;;
		
	\?)
	    echo "INVALID PARAMETER";
	    exit
	;;

     	e) 
	    INSTALL_ESPECIFIC="${TARGET}" 
	;;
 	esac
done

CREATE_WORKSPACE;
SUDO_UPDATE;

GET_INFORMATION;

INSTALL_COMMONS_DEPEND;
INSTALL_GUV;
INSTALL_OPENCV;
INSTALL_TGUI;
INSTALL_SFML;

cd ..
sudo rm -rf installation

#CLONE_PROJECT;

if [ $ERROR_OCURRED -eq 1 ]; then
	echo "${RED}${BOLD}SOME ERRORS OCURRED${NORMAL}";
fi
