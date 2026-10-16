"""SPOT audit game: read every chunk of the project, rewrite it or accept it.

Modules:
    chunker   cuts a file into [comment] -> [code] chunks, checks a rewrite's code
    catalog   the snapshot, the chapters and sections, every file's chunks
    progress  statuses, rewrites, flags, the log, achievements, the work copy
    server    the page and the JSON API
"""
