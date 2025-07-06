# HELLSPAWN OS

> Bare metal. No rules. RAM is the law.

Welcome to **HELLSPAWN OS** — a hobby operating system where every file is a struct, every mistake overwrites RAM, and every `make run` is a gamble.

No libc. No safety. Just raw C, assembly, and a burning desire to own every byte of memory between `0x0000` and `0xFFFFF`.

---

## ⚔️ What This Is

- A **real mode → protected mode** bootloader in x86 ASM.
- A **custom kernel** in C that doesn’t care about your segmentation faults.
- A **filesystem** (SLIM FS) that exists only in RAM and in vibes.

---

## 💾 SLIM FS

> **S**mall  
> **L**ightweight  
> **I**mplemented  
> **M**anagement  
> **f**ile  
> **s**ystem

SLIM FS stores your entire file system in RAM.  
No parsing. No disk. Just structs and raw memory ops.

### Files are just structs:

```c
struct fdata {
    uint8_t isdir;
    uint16_t id;
    uint8_t namelen;
    uint32_t ptrtodata;
    uint32_t childlenORfilesize;
    uint32_t data[];
};
````

Want to add a file? Just shove it in memory and pray.

---

## 🔥 Features

* Switches to 32-bit mode (goodbye real mode)
* Serial logging for debugging when things explode
* In-memory file system (SLIM FS)
* RAM-only malloc using `heap/mem-chunkX` files
* `newfile()`, `addchild()`, `findfile()` — your cursed FS API
* Works in QEMU and SeaBIOS (probably won't kill your host)

---

## 💀 How To Run & Build

### Build
```BASH
make
```

### Run
```bash
make run
```

Requirements:

* `nasm`
* `qemu`
* `i386-elf-gcc`
* `i386-elf-ld`
* A healthy disregard for stability
```BASH
sudo pacman -S nasm qemu-full
yay -S i386-elf-gcc
```

---

## ⚠️ Known Problems (Features?)

* If you touch memory wrong, the system dies — gloriously.
* Every file must be manually injected via `newfile()` calls in C.
* No disk I/O yet. FS is reset on every boot. Deal with it.

---

## 🤘 Philosophy

> C is not a language. It’s a dare.

HELLSPAWN OS isn’t here to protect you. It’s here to **let you write directly to address `0x0000`** and **call that the root directory**.
It’s a love letter to chaos. A middle finger to bloat.
A reminder that real devs write their own file system in RAM because they *can.*

---

## 🧙‍♂️ Author

**zalanwastaken**
Wielder of `0xBEEF`, destroyer of RAM, and builder of machines that should not work but somehow do.
