#pragma once

#define SAFE_RELEASE(r) {if (r) { (r)->Release(); (r) = nullptr; } }