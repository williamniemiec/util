package wniemiec.util.data;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import java.util.List;

import org.junit.Test;

public class StringUtilsTest {

	@Test
	public void testHelloWorld() {
		List<String> items = List.of("hello", "world");
		
		assertEquals("hello world", StringUtils.implode(items, " "));
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void testNullList() {
		StringUtils.implode(null, " ");
		
		fail();
	}

	@Test(expected = IllegalArgumentException.class)
	public void testNullDelimiter() {
		List<String> items = List.of("hello", "world");
		
		StringUtils.implode(items, null);
		
		fail();
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void testNullDelimiterAndList() {
		StringUtils.implode(null, null);
		
		fail();
	}
}
