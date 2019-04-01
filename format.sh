#!/usr/bin/env bash

CMD=clang-format-7
$CMD -version
$CMD -i -style=Google $(git ls-files|grep -E ".*\.(cc|h)$")
CHANGED="$(git ls-files --modified)"
if [[ ! -z "$CHANGED" ]]; then
  echo "The following files have changes:"
  echo "$CHANGED"
  exit 1
else
  echo "No changes."
fi