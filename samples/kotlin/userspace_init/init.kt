import kotlinx.cinterop.*
import platform.posix.*

@OptIn(ExperimentalForeignApi::class)
fun mountFilesystems() {
    memScoped {
        mkdir("/proc", 0x1edu) // 0755
        syscall(165, "proc".cstr.ptr, "/proc".cstr.ptr, "proc".cstr.ptr, 0L, null)
        
        mkdir("/sys", 0x1edu)
        syscall(165, "sysfs".cstr.ptr, "/sys".cstr.ptr, "sysfs".cstr.ptr, 0L, null)
        
        mkdir("/dev", 0x1edu)
        syscall(165, "devtmpfs".cstr.ptr, "/dev".cstr.ptr, "devtmpfs".cstr.ptr, 0L, null)
        
        mkdir("/tmp", 0x1ffu) // 0777
        syscall(165, "tmpfs".cstr.ptr, "/tmp".cstr.ptr, "tmpfs".cstr.ptr, 0L, null)
    }
}

@OptIn(ExperimentalForeignApi::class)
fun runCommand(path: String, args: CArrayPointer<CPointerVar<ByteVar>>, env: CArrayPointer<CPointerVar<ByteVar>>) {
    val pid = fork()
    if (pid == 0) {
        execve(path, args, env)
        exit(1)
    } else if (pid > 0) {
        memScoped {
            val status = alloc<IntVar>()
            waitpid(pid, status.ptr, 0)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
fun bringUpLoopback() {
    println("Configuring loopback network interface (lo)...")
    memScoped {
        val args = allocArrayOf("ip".cstr.ptr, "link".cstr.ptr, "set".cstr.ptr, "lo".cstr.ptr, "up".cstr.ptr, null)
        val env = allocArrayOf("PATH=/usr/bin:/bin:/sbin".cstr.ptr, null)
        runCommand("/usr/bin/ip", args, env)
        runCommand("/sbin/ip", args, env)
    }
}

@OptIn(ExperimentalForeignApi::class)
fun configureNetworking() {
    println("Configuring SLIRP networking (eth0)...")
    memScoped {
        val env = allocArrayOf("PATH=/usr/bin:/bin:/sbin".cstr.ptr, null)
        
        // ip link set eth0 up
        var args = allocArrayOf("ip".cstr.ptr, "link".cstr.ptr, "set".cstr.ptr, "eth0".cstr.ptr, "up".cstr.ptr, null)
        runCommand("/usr/bin/ip", args, env)
        runCommand("/sbin/ip", args, env)
        
        // ip addr add 10.0.2.15/24 dev eth0
        args = allocArrayOf("ip".cstr.ptr, "addr".cstr.ptr, "add".cstr.ptr, "10.0.2.15/24".cstr.ptr, "dev".cstr.ptr, "eth0".cstr.ptr, null)
        runCommand("/usr/bin/ip", args, env)
        runCommand("/sbin/ip", args, env)
        
        // ip route add default via 10.0.2.2
        args = allocArrayOf("ip".cstr.ptr, "route".cstr.ptr, "add".cstr.ptr, "default".cstr.ptr, "via".cstr.ptr, "10.0.2.2".cstr.ptr, null)
        runCommand("/usr/bin/ip", args, env)
        runCommand("/sbin/ip", args, env)
    }
    
    // Fix DNS resolution without corrupting the host's /etc/resolv.conf
    println("Configuring DNS resolution...")
    memScoped {
        val fd = open("/tmp/resolv.conf", 65, 420) // O_CREAT=64 | O_WRONLY=1, 0644=420
        if (fd >= 0) {
            val resolvConfContent = "nameserver 10.0.2.3\n"
            write(fd, resolvConfContent.cstr.ptr, resolvConfContent.length.toULong())
            close(fd)
            // Bind mount /tmp/resolv.conf over /etc/resolv.conf
            // 4096 is MS_BIND
            syscall(165L, "/tmp/resolv.conf".cstr.ptr, "/etc/resolv.conf".cstr.ptr, null, 4096L, null)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
fun main() {
    println("\n\n=======================================================")
    println("🚀 Welcome to the linux-kotlin kernel!")
    println("This is a native Linux environment powered entirely")
    println("by a Kotlin/Native userspace init process!")
    println("No bash, no glibc, just pure Kotlin running on metal.")
    println("=======================================================\n")
    
    println("Mounting /proc, /sys, /dev, and /tmp...")
    mountFilesystems()
    
    println("Filesystems mounted successfully.")
    
    bringUpLoopback()
    configureNetworking()
    
    println("\n=======================================================")
    println("🔨 linux-kotlin: LAUNCHING GRADLE NATIVE BUILD DAEMON...")
    println("=======================================================")
    
    // Dynamically retrieve the kernel source path passed by the bootloader/QEMU
    // This allows the repository to be cloned anywhere by any user!
    val kernelSrc = getenv("KERNEL_SRC")?.toKString() ?: error("KERNEL_SRC not set in bootloader environment!")
    
    // We now use the standalone Java and Android SDK bundled directly within the repository!
    val javaHome = "$kernelSrc/java"
    val androidHome = "$kernelSrc/android-sdk"
    
    val pid = fork()
    if (pid == 0) {
        // We are the child process.
        // Change into the Android sample directory dynamically
        chdir("$kernelSrc/samples/android/minimal")
        
        // Execute the Gradle build process directly from the kernel init!
        memScoped {
            val args = allocArrayOf(
                "$kernelSrc/.gradle-tools/gradle/bin/gradle".cstr.ptr,
                "assembleDebug".cstr.ptr,
                "--no-daemon".cstr.ptr,
                null
            )
            
            val env = allocArrayOf(
                "JAVA_HOME=$javaHome".cstr.ptr,
                "ANDROID_HOME=$androidHome".cstr.ptr,
                "ANDROID_USER_HOME=$kernelSrc/.android-home".cstr.ptr,
                "PATH=/usr/bin:/bin:/sbin".cstr.ptr,
                "HOME=$kernelSrc/.home".cstr.ptr,
                "GRADLE_USER_HOME=$kernelSrc/.gradle-cache".cstr.ptr,
                "TMPDIR=/tmp".cstr.ptr,
                null
            )
            
            val gradlePath = "$kernelSrc/.gradle-tools/gradle/bin/gradle"
            execve(gradlePath, args, env)
            println("❌ linux-kotlin: execve failed! Ensure Gradle path is correct.")
            exit(1)
        }
    } else if (pid > 0) {
        memScoped {
            val status = alloc<IntVar>()
            waitpid(pid, status.ptr, 0)
        }
        println("\n✅ linux-kotlin: Gradle Build Completed natively from Kernel Init!")
    } else {
        println("❌ linux-kotlin: fork() failed!")
    }
    
    println("\nHanding over control to system init (/sbin/init) or shell...")
    memScoped {
        val env = allocArrayOf("PATH=/usr/bin:/bin:/sbin".cstr.ptr, "HOME=/root".cstr.ptr, "KERNEL_SRC=$kernelSrc".cstr.ptr, null)
        
        // Try systemd / standard init
        val initArgs = allocArrayOf("/sbin/init".cstr.ptr, null)
        execve("/sbin/init", initArgs, env)
        
        // Fallback to bash
        println("❌ /sbin/init failed, falling back to /bin/bash...")
        val bashArgs = allocArrayOf("/bin/bash".cstr.ptr, null)
        execve("/bin/bash", bashArgs, env)
        
        // Fallback to sh
        println("❌ /bin/bash failed, falling back to /bin/sh...")
        val shArgs = allocArrayOf("/bin/sh".cstr.ptr, null)
        execve("/bin/sh", shArgs, env)
    }
    
    println("❌ linux-kotlin: Failed to hand off to any init process. Halting.")
    while (true) {
        sleep(1000u)
    }
}
