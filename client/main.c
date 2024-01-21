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




#define _POSIX_C_SOURCE 200809L /* strtok_r() */




#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <client_lib/functions.h>
#include <client_lib/rpc.h>
#include <client_lib/test_functions.h>




static void print_help(void)
{
	fprintf(stdout, "Commands executed with RPC:\n");
	fprintf(stdout, "    gettimeofday                 -- get current time\n");
	fprintf(stdout, "    add3 <int1> <int2> <int3>    -- add 3 integers\n");
	fprintf(stdout, "    sm_prev                      -- move state machine to prev state\n");
	fprintf(stdout, "    sm_next                      -- move state machine to next state\n");
	fprintf(stdout, "    sm_get_current_label         -- get label of current state\n");
	fprintf(stdout, "    sm_get_all_info              -- get all info about current state\n");
	fprintf(stdout, "    sm_set_label <idx> <label>   -- set label of specific state\n");
	fprintf(stdout, "Commands testing RPC mechanisms:\n");
	fprintf(stdout, "    transport_test               -- run tests of transport layer of RPC\n");
	fprintf(stdout, "Local commands:\n");
	fprintf(stdout, "    help / ?                     -- print this help text\n");
	fprintf(stdout, "    quit / q / exit              -- quit this program (but don't kill server)\n");
}




/**
   Get a list of tokens from stdin, to be interpreted as command+arguments in
   main loop.
*/
static int get_command(int * argc, char argv[10][1024], int tokens_max)
{
	char buffer[1024] = { 0 };
	fgets(buffer, sizeof (buffer), stdin);
	clearerr(stdin); /* To somehow handle "Ctrl+D". */
	if (0 == strlen(buffer)) {
		return -1;
	}


	if (ferror(stdin) || !isprint(buffer[0])) {
		snprintf(argv[0], sizeof (argv[0]), "%s", "quit");
		*argc = 1;
		return 0;
	}
	buffer[strlen(buffer) - 1] = '\0';

	char * saveptr = NULL;
	char * token = strtok_r(buffer, " ", &saveptr);
	*argc = 0;
	while (token) {
		if (*argc == tokens_max) {
			fprintf(stderr, "[EE] RPC client: too many tokens\n");
			return -1;
		}
		snprintf(argv[*argc], sizeof (argv[*argc]), "%s", token);
		(*argc)++;
		token = strtok_r(NULL, " ", &saveptr);
	}

	return 0;
}




/**
   Get command from stdin, run function corresponding to the command, print
   result. Repeat in forever loop.
*/
static int main_loop(const char * progname)
{
	bool run = true;
	while (run) {
		fprintf(stdout, "\n> "); /* Prompt. */
		fflush(stdout);

		char tokens[10][1024] = { 0 };
		int n_tokens = 0;
		const int n_tokens_max = 10;
		if (0 != get_command(&n_tokens, tokens, n_tokens_max)) {
			continue;
		}

		const char * command = tokens[0];
		if (0 == strcmp(command, "gettimeofday")) {
			if (n_tokens != 1) {
				fprintf(stderr, "[EE] %s: usage: 'gettimeofday'\n", progname);
				continue;
			}
			struct timeval tv = { 0 };
			if (0 != my_gettimeofday(&tv)) {
				fprintf(stderr, "[EE] %s: call to 'mygettimeofday' hs failed\n", progname);
				continue;
			}

			struct tm stm;
			char tmbuf[64];
			localtime_r((time_t *) &tv.tv_sec, &stm);
			strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", &stm);
			fprintf(stdout, "[II] %s: time = %s.%06ld\n", progname, tmbuf, tv.tv_usec);

		} else if (0 == strcmp(command, "add3")) {
			if (n_tokens != 4) {
				fprintf(stderr, "[EE] %s: usage: 'add3 <int1> <int2> <int3>'\n", progname);
				continue;
			}

			int a = atoi(tokens[1]);
			int b = atoi(tokens[2]);
			int c = atoi(tokens[3]);
			int result = 0;
			if (0 != my_add3(a, b, c, &result)) {
				fprintf(stderr, "[EE] %s: call to 'my_add3' has failed\n", progname);
				continue;
			}
			fprintf(stdout, "[II] %s: add3: %d + %d + %d = %d\n", progname, a, b, c, result);
		} else if (0 == strcmp(command, "sm_prev")) {
			if (n_tokens != 1) {
				fprintf(stderr, "[EE] %s: usage: 'sm_prev'\n", progname);
				continue;
			}
			if (0 != sm_prev()) {
				fprintf(stderr, "[EE] %s: call to 'sm_prev' has failed\n", progname);
				continue;
			}
		} else if (0 == strcmp(command, "sm_next")) {
			if (n_tokens != 1) {
				fprintf(stderr, "[EE] %s: usage: 'sm_next'\n", progname);
				continue;
			}
			if (0 != sm_next()) {
				fprintf(stderr, "[EE] %s: call to 'sm_next' has failed\n", progname);
				continue;
			}
		} else if (0 == strcmp(command, "sm_get_current_label")) {
			if (n_tokens != 1) {
				fprintf(stderr, "[EE] %s: usage: 'sm_get_current_label'\n", progname);
				continue;
			}

			char label[SM_STATE_LABEL_SIZE] = { 0 };
			if (0 != sm_get_current_label(label)) {
				fprintf(stderr, "[EE] %s: call to 'my_get_current_label' has failed\n", progname);
				continue;
			}
			fprintf(stdout, "[II] %s: label of current state is [%s]\n", progname, label);

		} else if (0 == strcmp(command, "sm_get_all_info")) {
			if (n_tokens != 1) {
				fprintf(stderr, "[EE] %s: usage: 'sm_get_all_info'\n", progname);
				continue;
			}

			struct sm_all_info_t info = { 0 };
			if (0 != sm_get_all_info(&info)) {
				fprintf(stderr, "[EE] %s: call to 'sm_get_all_info' has failed\n", progname);
				continue;
			}

			fprintf(stdout, "[II] %s: all info: state = [%d], timestamp = [%lu], label = [%s]\n",
				progname,
				info.state.sm_state,
				info.state.change_timestamp,
				info.label_arr);

		} else if (0 == strcmp(command, "sm_set_label")) {
			if (n_tokens != 3) {
				fprintf(stderr, "[EE] %s: usage: 'sm_set_label <idx> <label>'\n", progname);
				continue;
			}

			const int idx = atoi(tokens[1]);
			const char * label = tokens[2];

			if (idx < 1 || idx > 7) {
				fprintf(stderr, "[EE] %s: index must be in range 1-7\n", progname);
				continue;
			}

			if (0 != sm_set_label(label, idx)) {
				fprintf(stderr, "[EE] %s: call to 'sm_set_label' has failed\n", progname);
			}
		} else if (0 == strcmp(command, "transport_test")) {
			if (n_tokens != 1) {
				fprintf(stderr, "[EE] %s: usage: 'transport_test'\n", progname);
				continue;
			}
			if (0 == transport_test()) {
				fprintf(stdout, "[II] %s: transport test status: PASS\n", progname);
			} else {
				fprintf(stderr, "[EE] %s: transport test status: FAIL\n", progname);
			}
		} else if (0 == strcmp(command, "help") || 0 == strcmp(command, "?")) {
			print_help();
		} else if (0 == strcmp(command, "quit") || 0 == strcmp(command, "q") || 0 == strcmp(command, "exit")) {
			run = false;
		} else {
			fprintf(stderr, "[EE] %s: incorrect command: [%s]\n", progname, command);
		}
	}

	return 0;
}




/**
   Calls to library's RPC initialization and de-initialization functions are
   the only places where client code is involved in RPC.

   If we could hardcode the RPC server's address in library, we could run
   init and deinit functions in library's constructor/destructor (i.e.
   library's functions tagged with proper __attribute__()), and the client's
   code would be completely unaware of RPC.
*/
int main(int argc, char ** argv)
{
	const char * progname = argv[0];
 	if (argc < 2) {
		fprintf(stderr, "[EE] RPC client: usage: %s <rpc server>\n", progname);
		exit(EXIT_FAILURE);
	}

	const char * rpc_server = argv[1];
	if (0 != client_lib_rpc_init(rpc_server)) {
		fprintf(stderr, "[EE] RPC client: failed to initialize RPC\n");
		exit(EXIT_FAILURE);
	}

	main_loop("RPC client");

	client_lib_rpc_deinit();

	exit(EXIT_SUCCESS);
}


