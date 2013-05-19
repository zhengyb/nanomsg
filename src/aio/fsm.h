/*
    Copyright (c) 2013 250bpm s.r.o.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom
    the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#ifndef NN_FSM_INCLUDED
#define NN_FSM_INCLUDED

#include "../utils/queue.h"

/*  Base class for state machines. */

struct nn_ctx;
struct nn_fsm;
struct nn_worker;

struct nn_fsm_event {
    struct nn_fsm *fsm;
    void *source;
    int type;
    struct nn_queue_item item;
};

void nn_fsm_event_init (struct nn_fsm_event *self);
void nn_fsm_event_term (struct nn_fsm_event *self);
void nn_fsm_event_process (struct nn_fsm_event *self);

/*  Events generated by fsm object. */
#define NN_FSM_START 1
#define NN_FSM_STOP 2

/*  Virtual function to be implemented by the derived class to handle the
    incoming events. 'source' parameter is pointer to the object that generated
    the event. As it can be any kind of object it is of type void*.
    The user should check whether the pointer points to any source of
    events it is aware of and cast the pointer accordingly. If a single
    object can generate different kinds of events, 'type' parameter
    specifies the event type. Possible values are defined by the object
    that is the source of events. Source equal to NULL means the event is
    comming from the state machine owner. */
typedef void (*nn_fsm_fn) (struct nn_fsm *self, void *source, int type);

struct nn_fsm {
    nn_fsm_fn fn;
    struct nn_fsm *owner;
    struct nn_ctx *ctx;
};

void nn_fsm_init_root (struct nn_fsm *self, nn_fsm_fn fn, struct nn_ctx *ctx);
void nn_fsm_init (struct nn_fsm *self, nn_fsm_fn fn, struct nn_fsm *owner);
void nn_fsm_term (struct nn_fsm *self);

void nn_fsm_start (struct nn_fsm *self);
void nn_fsm_stop (struct nn_fsm *self);

struct nn_fsm *nn_fsm_swap_owner (struct nn_fsm *self, struct nn_fsm *newowner);

struct nn_worker *nn_fsm_choose_worker (struct nn_fsm *self);

/*  Send event from the state machine to its owner. */
void nn_fsm_raise (struct nn_fsm *self, struct nn_fsm_event *event,
    void *source, int type);

#endif

