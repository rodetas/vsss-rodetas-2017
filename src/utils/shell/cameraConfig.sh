#!/bin/bash

LIST_CAMERA=`ls /dev/video* | wc -l`
# echo $LIST_CAMERA

GET_CAMERAS () {
	if [ ${LIST_CAMERA} -eq 0 ]; then
		exit -1
	elif [ ${LIST_CAMERA} -eq 1 ]; then
		exit 0
	elif [ ${LIST_CAMERA} -eq 2 ]; then
		exit 1
	fi

}

CONFIGURE_CAMERA () {

	if [ ${LIST_CAMERA} -eq 0 ]; then
		echo "NO CAMERA CONNECTED"

	elif [ ${LIST_CAMERA} -eq 1 ]; then
		CAMERA="video0"

	else
		CAMERA="video1"
	fi

	echo "Configuring ${CAMERA}..."

	uvcdynctrl -d ${CAMERA} -v -s 'Brightness' 130
	uvcdynctrl -d ${CAMERA} -v -s 'Contrast' 150
	uvcdynctrl -d ${CAMERA} -v -s 'Saturation' 190
	uvcdynctrl -d ${CAMERA} -v -s 'White Balance Temperature, Auto' 0
	uvcdynctrl -d ${CAMERA} -v -s 'Gain' 50
	uvcdynctrl -d ${CAMERA} -v -s 'Sharpness' 128 #Nitidez
	uvcdynctrl -d ${CAMERA} -v -s 'Backlight Compensation' 0
	uvcdynctrl -d ${CAMERA} -v -s 'Exposure, Auto' 1
	uvcdynctrl -d ${CAMERA} -v -s 'Exposure (Absolute)' 150
	uvcdynctrl -d ${CAMERA} -v -s 'Exposure, Auto Priority' 0
	uvcdynctrl -d ${CAMERA} -v -s 'Focus, Auto' 0

	exit $LIST_CAMERA
}

if [ $# -gt 1 ]; then
	echo "INCORRECT NUMBERS OF PARAMETERS"

elif [ $1 == "get_camera" ]; then
	GET_CAMERAS;

elif [ $1 == "configure" ]; then
	CONFIGURE_CAMERA;

fi