#include "gmock/gmock.h"
#include "device_driver.h"

using namespace testing;

class FakeFlashMemoryDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long), (override));
	MOCK_METHOD(void, write, (long, unsigned char), (override));
};

class DeviceDriverFixture : public Test {
public:
	NiceMock<FakeFlashMemoryDevice> mock_hardware;
};

TEST_F(DeviceDriverFixture, TestRead) {
	EXPECT_CALL(mock_hardware, read(0x8))
		.Times(5)
		.WillRepeatedly(Return(0x88));

	DeviceDriver dd{ &mock_hardware };
	EXPECT_EQ(0x88, dd.read((long)0x8));
}

TEST_F(DeviceDriverFixture, TestReadAndException) {
	EXPECT_CALL(mock_hardware, read(0x8))
		.WillOnce(Return((unsigned char)0xA))
		.WillOnce(Return((unsigned char)0xA))
		.WillOnce(Return((unsigned char)0xA))
		.WillOnce(Return((unsigned char)0xA))
		.WillOnce(Return((unsigned char)0xB));

	try {
		DeviceDriver dd{ &mock_hardware };
		int data = dd.read((long)0x8);
		FAIL();
	}
	catch (ReadFailException& e) {
		EXPECT_EQ(std::string{ e.what() }, std::string{ "Read failed because read data is not same 5 times" });
	}
}

TEST_F(DeviceDriverFixture, TestWrite) {
	EXPECT_CALL(mock_hardware, read(0xB))
		.Times(1);

	DeviceDriver dd{ &mock_hardware };
	dd.write(0xB, 0x37);
}

TEST_F(DeviceDriverFixture, TestWriteAndException) {
	EXPECT_CALL(mock_hardware, read(0xB))
		.Times(1)
		.WillOnce(Return(1));

	try {
		DeviceDriver dd{ &mock_hardware };
		dd.write(0xB, 0x37);
	}
	catch (WriteFailException& e) {
		EXPECT_EQ(std::string{ e.what() }, std::string{ "Write failed because it has valid data" });
	}
}

