// SPDX-License-Identifier: GPL-2.0

#include <linux/clk.h>

/*
 * The "inline" implementation of below helpers are only available when
 * CONFIG_HAVE_CLK or CONFIG_HAVE_CLK_PREPARE aren't set.
 */
#ifndef CONFIG_HAVE_CLK
<<<<<<< HEAD
__rust_helper struct clk *rust_helper_clk_get(struct device *dev,
					      const char *id)
=======
struct clk *rust_helper_clk_get(struct device *dev, const char *id)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return clk_get(dev, id);
}

<<<<<<< HEAD
__rust_helper void rust_helper_clk_put(struct clk *clk)
=======
void rust_helper_clk_put(struct clk *clk)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	clk_put(clk);
}

<<<<<<< HEAD
__rust_helper int rust_helper_clk_enable(struct clk *clk)
=======
int rust_helper_clk_enable(struct clk *clk)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return clk_enable(clk);
}

<<<<<<< HEAD
__rust_helper void rust_helper_clk_disable(struct clk *clk)
=======
void rust_helper_clk_disable(struct clk *clk)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	clk_disable(clk);
}

<<<<<<< HEAD
__rust_helper unsigned long rust_helper_clk_get_rate(struct clk *clk)
=======
unsigned long rust_helper_clk_get_rate(struct clk *clk)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return clk_get_rate(clk);
}

<<<<<<< HEAD
__rust_helper int rust_helper_clk_set_rate(struct clk *clk, unsigned long rate)
=======
int rust_helper_clk_set_rate(struct clk *clk, unsigned long rate)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return clk_set_rate(clk, rate);
}
#endif

#ifndef CONFIG_HAVE_CLK_PREPARE
<<<<<<< HEAD
__rust_helper int rust_helper_clk_prepare(struct clk *clk)
=======
int rust_helper_clk_prepare(struct clk *clk)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return clk_prepare(clk);
}

<<<<<<< HEAD
__rust_helper void rust_helper_clk_unprepare(struct clk *clk)
=======
void rust_helper_clk_unprepare(struct clk *clk)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	clk_unprepare(clk);
}
#endif

<<<<<<< HEAD
__rust_helper struct clk *rust_helper_clk_get_optional(struct device *dev,
						       const char *id)
=======
struct clk *rust_helper_clk_get_optional(struct device *dev, const char *id)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return clk_get_optional(dev, id);
}

<<<<<<< HEAD
__rust_helper int rust_helper_clk_prepare_enable(struct clk *clk)
=======
int rust_helper_clk_prepare_enable(struct clk *clk)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return clk_prepare_enable(clk);
}

<<<<<<< HEAD
__rust_helper void rust_helper_clk_disable_unprepare(struct clk *clk)
=======
void rust_helper_clk_disable_unprepare(struct clk *clk)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	clk_disable_unprepare(clk);
}
