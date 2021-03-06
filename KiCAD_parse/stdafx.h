// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <unordered_map>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/detail/sp_nullptr_t.hpp>

#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;
using bfs_vect_type = std::vector<bfs::path>;

#include <boost/algorithm/string.hpp>
#include "typeswitch.h"



// TODO: reference additional headers your program requires here
