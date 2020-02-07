/***
  This file is part of catta.

  catta is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  catta is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
  Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with catta; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <stdlib.h>

#include <catta/timeval.h>
#include <catta/simple-watch.h>
#include <catta/log.h>

#include "../src/timeeventq.h"

#define POINTER_TO_INT(p) ((int) (long) (p))
#define INT_TO_POINTER(i) ((void*) (long) (i))

static CattaTimeEventQueue *q = NULL;

static void callback(CattaTimeEvent*e, void* userdata) {
    struct timeval tv = {0, 0};
    assert(e);
    catta_log_info("callback(%i)", POINTER_TO_INT(userdata));
    catta_elapse_time(&tv, 1000, 100);
    catta_time_event_update(e, &tv);
}

int main(CATTA_GCC_UNUSED int argc, CATTA_GCC_UNUSED char *argv[]) {
    struct timeval tv;
    CattaSimplePoll *s;

    s = catta_simple_poll_new();

    q = catta_time_event_queue_new(catta_simple_poll_get(s));

    catta_time_event_new(q, catta_elapse_time(&tv, 5000, 100), callback, INT_TO_POINTER(1));
    catta_time_event_new(q, catta_elapse_time(&tv, 5000, 100), callback, INT_TO_POINTER(2));

    catta_log_info("starting");

    for (;;)
        if (catta_simple_poll_iterate(s, -1) != 0)
            break;

    catta_time_event_queue_free(q);
    catta_simple_poll_free(s);

    return 0;
}