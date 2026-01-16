# 🔥 HellFireOS 🔥
> a handcrafted OS because **why not** 😈

HellFireOS is a from-scratch hobby operating system written in **C + x86 ASM**.
No libc. No BIOS hand-holding. Just raw CPU vibes.

This project exists because:
- learning OS dev is cool
- pain is temporary
- serial logs are forever

---

## ✨ Current Features
- ✅ Custom bootloader
- ✅ Protected mode (32-bit)
- ✅ Serial output (COM1)
- ⌨️ Keyboard **(WIP)**

---

## ❌ Not Implemented (Yet)
- ❌ Filesystem
- ❌ Memory allocator (`malloc/free`)
- ❌ Syscalls
- ❌ User mode
- ❌ ELF loading
- ❌ libc / `unistd.h`

---

## 🧪 Running (QEMU)
```bash
make
make run
````

Serial output is your best friend 🫂

---

## 🧭 Roadmap

* [ ] Keyboard driver
* [ ] Simple heap allocator
* [ ] Basic filesystem
* [ ] Syscall interface
* [ ] Userspace

---

## ⚠️ Disclaimer

This OS is:

* not secure
* not stable
* not fast
* not POSIX
* not sane

But it **boots**, and that’s what matters 💪🔥

---

## 💖 Why?

Because writing an OS teaches you more than 100 tutorials ever will.

Also because it’s funny.
