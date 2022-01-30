/*
 * Copyright (c) 2022 Andri Yngvason
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once

#include <stdbool.h>

// TODO: Add a way to set the rate limit

struct zext_screencopy_manager_v1;
struct wl_output;
struct wv_buffer;

enum screencopy_result {
	SCREENCOPY_DONE,
	SCREENCOPY_FATAL,
	SCREENCOPY_FAILED,
};

typedef void (*screencopy_done_fn)(enum screencopy_result,
		struct wv_buffer* buffer, void* userdata);

struct screencopy_impl {
	struct screencopy* (*create)(struct wl_output*, bool render_cursor);
	struct screencopy* (*create_cursor)(struct wl_output*);
	void (*destroy)(struct screencopy*);
	int (*start)(struct screencopy*, bool immediate);
	void (*stop)(struct screencopy*);
};

struct screencopy {
	struct screencopy_impl* impl;

	double rate_limit;
	bool enable_linux_dmabuf;

	screencopy_done_fn on_done;
	void (*cursor_enter)(void* userdata);
	void (*cursor_leave)(void* userdata);
	void (*cursor_hotspot)(int x, int y, void* userdata);

	void* userdata;
};

struct screencopy* screencopy_create(struct screencopy_impl* impl,
		struct wl_output* output, bool render_cursor);
struct screencopy* screencopy_create_cursor(struct screencopy_impl* impl,
		struct wl_output* output);
void screencopy_destroy(struct screencopy* self);

int screencopy_start(struct screencopy* self, bool immediate);
void screencopy_stop(struct screencopy* self);
