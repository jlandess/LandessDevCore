#include "options.h"
#include "reproc.h"
#include "error.h"
#include "options.h"

static bool redirect_is_set(reproc_redirect redirect)
{
  return redirect.type || redirect.handle || redirect.file || redirect.path;
}

static int parse_redirect(reproc_redirect *redirect,
                          REPROC_STREAM stream,
                          bool parent,
                          bool discard,
                          FILE *file,
                          const char *path)
{
  ASSERT(redirect);

  if (file) {
    ASSERT_EINVAL(!redirect_is_set(*redirect));
    ASSERT_EINVAL(!parent && !discard && !path);
    redirect->type = REPROC_REDIRECT_FILE;
    redirect->file = file;
  }

  if (path) {
    ASSERT_EINVAL(!redirect_is_set(*redirect));
    ASSERT_EINVAL(!parent && !discard && !file);
    redirect->type = REPROC_REDIRECT_PATH;
    redirect->path = path;
  }

  if (redirect->type == REPROC_REDIRECT_HANDLE || redirect->handle) {
    ASSERT_EINVAL(!redirect->type || redirect->type == REPROC_REDIRECT_HANDLE);
    ASSERT_EINVAL(redirect->handle);
    ASSERT_EINVAL(!redirect->file && !redirect->path);
    redirect->type = REPROC_REDIRECT_HANDLE;
  }

  if (redirect->type == REPROC_REDIRECT_FILE || redirect->file) {
    ASSERT_EINVAL(!redirect->type || redirect->type == REPROC_REDIRECT_FILE);
    ASSERT_EINVAL(redirect->file);
    ASSERT_EINVAL(!redirect->handle && !redirect->path);
    redirect->type = REPROC_REDIRECT_FILE;
  }

  if (redirect->type == REPROC_REDIRECT_PATH || redirect->path) {
    ASSERT_EINVAL(!redirect->type || redirect->type == REPROC_REDIRECT_PATH);
    ASSERT_EINVAL(redirect->path);
    ASSERT_EINVAL(!redirect->handle && !redirect->file);
    redirect->type = REPROC_REDIRECT_PATH;
  }

  if (!redirect->type) {
    if (parent) {
      ASSERT_EINVAL(!discard);
      redirect->type = REPROC_REDIRECT_PARENT;
    } else if (discard) {
      ASSERT_EINVAL(!parent);
      redirect->type = REPROC_REDIRECT_DISCARD;
    } else {
      redirect->type = stream == REPROC_STREAM_ERR ? REPROC_REDIRECT_PARENT
                                                   : REPROC_REDIRECT_PIPE;
    }
  }

  return 0;
}

int parse_options(reproc_options *options, const char *const *argv)
{
  ASSERT(options);

  int r = -1;

  r = parse_redirect(&options->redirect.in, REPROC_STREAM_IN,
                     options->redirect.parent, options->redirect.discard, NULL,
                     NULL);
  if (r < 0) {
    return r;
  }

  r = parse_redirect(&options->redirect.out, REPROC_STREAM_OUT,
                     options->redirect.parent, options->redirect.discard,
                     options->redirect.file, options->redirect.path);
  if (r < 0) {
    return r;
  }

  r = parse_redirect(&options->redirect.err, REPROC_STREAM_ERR,
                     options->redirect.parent, options->redirect.discard,
                     options->redirect.file, options->redirect.path);
  if (r < 0) {
    return r;
  }

  if (options->input.data != NULL || options->input.size > 0) {
    ASSERT_EINVAL(options->input.data != NULL);
    ASSERT_EINVAL(options->input.size > 0);
    ASSERT_EINVAL(options->redirect.in.type == REPROC_REDIRECT_PIPE);
  }

  if (options->fork) {
    ASSERT_EINVAL(argv == NULL);
  } else {
    ASSERT_EINVAL(argv != NULL && argv[0] != NULL);
  }

  if (options->deadline == 0) {
    options->deadline = REPROC_INFINITE;
  }

  bool is_noop = options->stop.first.action == REPROC_STOP_NOOP &&
                 options->stop.second.action == REPROC_STOP_NOOP &&
                 options->stop.third.action == REPROC_STOP_NOOP;

  if (is_noop) {
    options->stop.first.action = REPROC_STOP_WAIT;
    options->stop.first.timeout = REPROC_DEADLINE;
    options->stop.second.action = REPROC_STOP_TERMINATE;
    options->stop.second.timeout = REPROC_INFINITE;
  }

  return 0;
}
