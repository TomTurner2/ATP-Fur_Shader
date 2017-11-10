#pragma once

#define SAFE_RELEASE(r) {if (r != nullptr) { (r)->Release(); (r) = nullptr; } }
#define SAFE_DELETE(r) {if (r != nullptr) { delete (r); (r) = nullptr; } }