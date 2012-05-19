/***
 * @file lightstone.c
 * @brief Common functions for lightstone communication
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2006-2012 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://liblightstone.nonpolynomial.com/
 *
 */

/* #include "lightstone/lightstone.h" */
#include <hidapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define E_REALTOUCH_DRIVER_ERROR -1
#define E_REALTOUCH_NOT_INITED -2
#define E_REALTOUCH_NOT_OPENED -3



#define REALTOUCH_DECLSPEC
#define REALTOUCH_VID 0x1f54
#define REALTOUCH_PID 0x0001

/**
 * Opaque structure to hold information about HID devices.
 *
 * @ingroup CoreFunctions
 */
struct realtouch {
	/// hidapi device handle
	hid_device* _dev;
	/// 0 if device is closed, > 0 otherwise
	int _is_open;
	/// 0 if device is initialized, > 0 otherwise
	int _is_inited;
};

REALTOUCH_DECLSPEC struct realtouch* realtouch_create()
{
	struct realtouch* s = (struct realtouch*)malloc(sizeof(struct realtouch));
	s->_is_open = 0;
	s->_is_inited = 0;
	hid_init();
	s->_is_inited = 1;	
	return s;
}

REALTOUCH_DECLSPEC int realtouch_get_count(struct realtouch* s)
{
	int count = 0;
	struct hid_device_info* devices_old;
	struct hid_device_info* devices_new;
	struct hid_device_info* device_cur;
	if (!s->_is_inited)
	{
		return E_REALTOUCH_NOT_INITED;
	}
	devices_new = hid_enumerate(REALTOUCH_VID, REALTOUCH_PID);

	device_cur = devices_new;
	while(device_cur) {
		++count;
		device_cur = device_cur->next;
	}
	
	hid_free_enumeration(devices_new);	
	return count;
}

REALTOUCH_DECLSPEC int realtouch_open(struct realtouch* s, unsigned int device_index)
{
	int count = 0;
	struct hid_device_info* devices_new;
	struct hid_device_info* device_cur;
	if (!s->_is_inited)
	{
		return E_REALTOUCH_NOT_INITED;
	}
	devices_new = hid_enumerate(REALTOUCH_VID, REALTOUCH_PID);


	device_cur = devices_new;
	while(device_cur) {
		if(count == device_index) {
			s->_dev = hid_open_path(device_cur->path);
			break;
		}
		++count;
		device_cur = device_cur->next;
	}

	hid_free_enumeration(devices_new);
	if(!s->_dev) {
		return E_REALTOUCH_NOT_OPENED;
	}
	s->_is_open = 1;
	return 0;
}

REALTOUCH_DECLSPEC int realtouch_close(struct realtouch* s)
{
	if(!s->_is_open)
	{
		return E_REALTOUCH_NOT_OPENED;
	}
	hid_close(s->_dev);
	s->_is_open = 0;
	return 0;
}

REALTOUCH_DECLSPEC void realtouch_delete(struct realtouch* dev)
{
	free(dev);
	dev = NULL;
	hid_exit();
}

int realtouch_read(struct realtouch* dev, unsigned int size, unsigned char* input_report)
{
	return hid_read(dev->_dev, input_report, size);
}

int realtouch_write(struct realtouch* dev, unsigned int size, unsigned char* input_report)
{
	return hid_write(dev->_dev, input_report, size);
}

REALTOUCH_DECLSPEC unsigned int hex2dec(char *data, unsigned int len)
{
	unsigned int i;
	unsigned int value = 0;

	for (i = 0; i < len; i++) {
		value = value*0x10;
		if (data[i] >= '0' && data[i] <= '9')
			value += data[i]-'0';
		else if (data[i] >= 'A' && data[i] <= 'F')
			value += data[i]-'A' + 10;
		else
			return 0;
	}
	return value;
}

REALTOUCH_DECLSPEC int realtouch_valid(struct realtouch* d)
{	
	return d->_is_open;
}

int main(int argc, char** argv)
{
	struct realtouch* test = realtouch_create();
	int ret, count, i, j;

	uint8_t msg[64];
	memset(msg, 0, 64);
	count = realtouch_get_count(test);

	if(!count)
	{
		printf("No realtouchs connected!\n");
		return 1;
	}
	printf("Found %d Realtouchs\n", count);
	printf("Opening realtouch %d\n", 0);
	ret = realtouch_open(test, 0);
	if(ret < 0)
	{
		printf("Cannot open realtouch!\n");
		return 1;
	}		

	//Get the firmware version
	realtouch_write(test, 64, msg);
	int r = realtouch_read(test, 64, msg);
	printf("Version reply:\n");
	for(i = 0; i < 3; ++i)
		printf("0x%.02x ", msg[i]);
	printf("\n");

	// Get serial number
	// Byte 0: length
	// Byte 3-4: Clock values for auth, now unused
	// Byte 7-9: Serial number
	memset(msg, 0, 64);
	msg[0] = 0xa;
	realtouch_write(test, 64, msg);
	r = realtouch_read(test, 64, msg);
	printf("Serial reply:\n");
	for(i = 0; i < msg[0]; ++i)
		printf("0x%.02x ", msg[i]);
	printf("\n");


	realtouch_close(test);
	printf("Closed realtouch\n");
	realtouch_delete(test);
	return 0;
	
}
