SECTIONS
{
<<<<<<< HEAD
	. = ALIGN(65536);
	.note.gnu.build-id : { *(.note.gnu.build-id) }
	. = ALIGN(65536);
=======
	. = ALIGN(4096);
	.note.gnu.build-id : { *(.note.gnu.build-id) }
	. = ALIGN(4096);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}
INSERT AFTER .text;

build_id_start = ADDR(.note.gnu.build-id);
build_id_end = ADDR(.note.gnu.build-id) + SIZEOF(.note.gnu.build-id);

