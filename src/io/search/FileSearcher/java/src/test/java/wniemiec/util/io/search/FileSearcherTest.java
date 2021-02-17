package wniemiec.util.io.search;

import static org.junit.Assert.assertEquals;

import java.io.IOException;
import java.nio.file.Path;

import org.junit.Test;

public class FileSearcherTest {

	//-------------------------------------------------------------------------
	//		Attributes
	//-------------------------------------------------------------------------
	private static final Path TESTS_FOLDER;
	
	
	//-------------------------------------------------------------------------
	//		Initialization block
	//------------------------------------------------------------------------
	static {
		TESTS_FOLDER = Path.of(".", "src", "test", "java");
	}
	
	
	//-------------------------------------------------------------------------
	//		Tests
	//------------------------------------------------------------------------
	@Test
	public void testSearchFile() throws IOException {
		FileSearcher searcher = new FileSearcher(TESTS_FOLDER);
		Path thisFile = TESTS_FOLDER.resolve(Path.of("wniemiec", "util", "io", "search", 
													"FileSearcherTest.java"));
		
		assertEquals(thisFile.toAbsolutePath(), searcher.search("FileSearcherTest.java"));
	}

	@Test(expected = IllegalArgumentException.class)
	public void testConstructorNullWorkingDirectory() {
		new FileSearcher(null);
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void testSearchNullFilename() throws IOException {
		FileSearcher searcher = new FileSearcher(TESTS_FOLDER);
		
		searcher.search(null);
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void testSearchEmptyFilename() throws IOException {
		FileSearcher searcher = new FileSearcher(TESTS_FOLDER);
		
		searcher.search("");
	}
}
