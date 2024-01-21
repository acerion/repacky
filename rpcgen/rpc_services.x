/*
  repacky - my toy RPC code
  Copyright (C) 2024 Kamil Ignacak

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/




/*
  My rpc services.
*/




/* ======== gettimeofday */

struct my_timeval_t {
       int rv; /* Resturn value of gettimeofday(). */
       unsigned int tv_sec;
       unsigned int tv_usec;
};





/* ======== State machine. */

enum sm_state_e {
     sm_state_init      = 0,
     sm_state_monday    = 1,
     sm_state_tuesday   = 2,
     sm_state_wednesday = 3,
     sm_state_thursday  = 4,
     sm_state_friday    = 5,
     sm_state_saturday  = 6,
     sm_state_sunday    = 7
};

struct sm_state_t {
       enum sm_state_e sm_state;

       /* Timestamp of last change of state. Type for C language: time_t. */
       u_long change_timestamp;
};

const SM_STATE_LABEL_SIZE = 10;
typedef opaque sm_label_arr_t[SM_STATE_LABEL_SIZE];
typedef string sm_label_t<SM_STATE_LABEL_SIZE>;

struct sm_all_info_t {
       sm_state_t state;
       sm_label_arr_t label_arr;
};




/* ======== Test code. */

const TEST_THREADS_COUNT = 3;
const TEST_DATA_SIZE = 32;

typedef string test_str_t<TEST_DATA_SIZE>;
typedef opaque test_arr_t[TEST_DATA_SIZE];





/* ======== The program definition. */

/* Our program is called MY_RPC_SERVER. */
program MY_RPC_SERVER {
	version MY_RPC_FUNCTIONS {
		my_timeval_t FN_GETTIMEOFDAY() = 1;
		int FN_ADD3(int, int, int) = 2;

		sm_state_t FN_SM_PREV() = 3;
		sm_state_t FN_SM_NEXT() = 4;
		sm_label_arr_t FN_SM_SET_LABEL(sm_label_t, int) = 5;
		sm_label_arr_t FN_SM_GET_CURRENT_LABEL() = 6;
		sm_all_info_t FN_SM_GET_ALL_INFO() = 7;

		int FN_TRANSPORT_TEST_SET(test_str_t, int) = 8;
		test_arr_t FN_TRANSPORT_TEST_GET(int) = 9;
	} = 1;
} = 0x20002024;

