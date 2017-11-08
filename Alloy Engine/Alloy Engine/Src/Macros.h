#pragma once

#define SAFE_RELEASE(r) {if (r) { (r)->Release(); (r) = nullptr; } }
#define SAFE_DELETE(r) {if (r) { delete (r); (r) = nullptr; } }