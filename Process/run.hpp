#pragma once

#include "drain.hpp"
#include "reproc.hpp"

namespace reproc {

template <typename Out, typename Err>
std::pair<int, std::error_code>
run(const arguments &arguments, const options &options, Out &&out, Err &&err)
{
  process process;
  std::error_code ec;

  ec = process.start(arguments, options);
  if (ec) {
    return { -1, ec };
  }

  ec = drain(process, std::forward<Out>(out), std::forward<Err>(err));
  if (ec) {
    return { -1, ec };
  }

  return process.wait(reproc::deadline);
}

inline std::pair<int, std::error_code> run(const arguments &arguments,
                                           const options &options = {})
{
  struct options modified = options::clone(options);

  if (!options.redirect.discard && options.redirect.file == nullptr &&
      options.redirect.path == nullptr) {
    modified.redirect.parent = true;
  }

  return run(arguments, modified, sink::null, sink::null);
}

}
