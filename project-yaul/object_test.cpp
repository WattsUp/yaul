#include <yaul/object.hpp>

#include "common/string.hpp"
#include "object_impl.hpp"

#include <gtest/gtest.h>
#include <algorithm>

/******************************** Object Tests ********************************/

class TestObject final : public ::yaul::Object {
 public:
  class Impl final : public ::yaul::Object::Impl {
   private:
    int* testValue = nullptr;

   public:
    static bool implementationDeleted;

    Impl() noexcept { implementationDeleted = false; }

    YAUL_DEFINE_DESTRUCT_MOVE_COPY(Impl);

    [[nodiscard]] inline int* getTestValue() const noexcept {
      return testValue;
    }

    inline void giveTestValue(int* testValue) noexcept {
      this->testValue = testValue;
    }
  };

  TestObject() noexcept;

  YAUL_DEFINE_DESTRUCT_MOVE_COPY(TestObject);

  [[nodiscard]] inline int* getTestValue() const noexcept {
    return impl<Impl>()->getTestValue();
  }

  inline void giveTestValue(int* testValue) noexcept {
    impl<Impl>()->giveTestValue(testValue);
  }
};

bool TestObject::Impl::implementationDeleted = false;

TestObject::TestObject() noexcept
    : ::yaul::Object(std::make_unique<TestObject::Impl>()) {}

YAUL_IMPL_DESTRUCT_MOVE_COPY(TestObject, ::yaul::Object);

TestObject::Impl::~Impl() noexcept {
  EXPECT_FALSE(implementationDeleted);
  delete testValue;
  implementationDeleted = true;
}

TestObject::Impl::Impl(TestObject::Impl&& o) noexcept {
  std::swap(testValue, o.testValue);
}

TestObject::Impl& TestObject::Impl::operator=(TestObject::Impl&& o) noexcept {
  std::swap(testValue, o.testValue);
  return *this;
}

TestObject::Impl::Impl(const TestObject::Impl& o) noexcept
    : testValue(new int(*o.testValue)) {}
TestObject::Impl& TestObject::Impl::operator=(
    const TestObject::Impl& o) noexcept {
  delete testValue;
  testValue = new int(*o.testValue);
  return *this;
}

class Object : public ::testing::Test {
 protected:
  ::yaul::string initialConfig;

  int testValue;
  TestObject* object;

  virtual void SetUp() {
    testValue = rand();
    object    = new TestObject();
    object->giveTestValue(new int(testValue));

    initialConfig += "  testValue   = " + std::to_string(testValue) + ";\n";
  }

  virtual void TearDown() {
    delete object;
    if (object != nullptr)
      EXPECT_TRUE(TestObject::Impl::implementationDeleted);

    if (::testing::Test::HasFailure()) {
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_RED,
                                         "Failing Test Configuration\n");
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_DEFAULT, "%s",
                                         initialConfig.c_str());
    }
  }
};

TEST_F(Object, Constructor) {
  ASSERT_TRUE(object->implSet());
  ASSERT_NE(object->getTestValue(), nullptr);
  EXPECT_EQ(*object->getTestValue(), testValue);
}

TEST_F(Object, CopyConstructor) {
  {
    TestObject copy(*object);

    ASSERT_TRUE(object->implSet());
    ASSERT_NE(object->getTestValue(), nullptr);

    ASSERT_TRUE(copy.implSet());
    ASSERT_NE(copy.getTestValue(), nullptr);

    ASSERT_NE(object->getTestValue(), copy.getTestValue());

    EXPECT_EQ(*object->getTestValue(), *copy.getTestValue());
  }
  // copy gets deleted when it goes out of scope so the implementation should
  // get deleted because it is not shared with object
  EXPECT_TRUE(TestObject::Impl::implementationDeleted);

  // Reset for the object's implementation deletion check
  TestObject::Impl::implementationDeleted = false;
}

TEST_F(Object, CopyAssignment) {
  {
    TestObject copy{};
    copy = *object;

    // Original copy should be deleted
    ASSERT_TRUE(TestObject::Impl::implementationDeleted);
    TestObject::Impl::implementationDeleted = false;

    ASSERT_TRUE(object->implSet());
    ASSERT_NE(object->getTestValue(), nullptr);

    ASSERT_TRUE(copy.implSet());
    ASSERT_NE(copy.getTestValue(), nullptr);

    ASSERT_NE(object->getTestValue(), copy.getTestValue());

    EXPECT_EQ(*object->getTestValue(), *copy.getTestValue());
  }
  // copy gets deleted when it goes out of scope so the implementation should
  // get deleted because it is not shared with object
  EXPECT_TRUE(TestObject::Impl::implementationDeleted);

  // Reset for the object's implementation deletion check
  TestObject::Impl::implementationDeleted = false;
}

TEST_F(Object, MoveConstructor) {
  {
    TestObject destination(std::move(*object));
    ASSERT_FALSE(object->implSet());
    ASSERT_TRUE(destination.implSet());

    ASSERT_NE(destination.getTestValue(), nullptr);

    EXPECT_EQ(*destination.getTestValue(), testValue);
  }
  // destination gets deleted when it goes out of scope so the implementation
  // should get deleted because it is not shared with object
  EXPECT_TRUE(TestObject::Impl::implementationDeleted);

  // Reset for the object's implementation deletion check
  TestObject::Impl::implementationDeleted = false;

  // Since object doesn't have any implementation, deleting should not delete
  // any implementation
  delete object;
  object = nullptr;
  EXPECT_FALSE(TestObject::Impl::implementationDeleted);
}

TEST_F(Object, MoveAssignment) {
  {
    TestObject destination{};
    destination = std::move(*object);

    // Original destination should be deleted
    ASSERT_TRUE(TestObject::Impl::implementationDeleted);
    TestObject::Impl::implementationDeleted = false;

    ASSERT_FALSE(object->implSet());
    ASSERT_TRUE(destination.implSet());

    ASSERT_NE(destination.getTestValue(), nullptr);

    EXPECT_EQ(*destination.getTestValue(), testValue);
  }
  // destination gets deleted when it goes out of scope so the implementation
  // should get deleted because it is not shared with object
  EXPECT_TRUE(TestObject::Impl::implementationDeleted);

  // Reset for the object's implementation deletion check
  TestObject::Impl::implementationDeleted = false;

  // Since object doesn't have any implementation, deleting should not delete
  // any implementation
  delete object;
  object = nullptr;
  EXPECT_FALSE(TestObject::Impl::implementationDeleted);
}

/**************************** Shared Object Tests *****************************/

class TestSharedObject final : public ::yaul::SharedObject {
 public:
  class Impl final : public ::yaul::SharedObject::Impl {
   private:
    int* testValue = nullptr;

   public:
    static bool implementationDeleted;

    Impl() noexcept { implementationDeleted = false; }

    YAUL_DEFINE_DESTRUCT_MOVE_COPY(Impl);

    [[nodiscard]] inline int* getTestValue() const noexcept {
      return testValue;
    }

    inline void giveTestValue(int* testValue) noexcept {
      this->testValue = testValue;
    }
  };

  TestSharedObject() noexcept;

  YAUL_DEFINE_DESTRUCT_MOVE_COPY(TestSharedObject);

  [[nodiscard]] inline int* getTestValue() const noexcept {
    return impl<Impl>()->getTestValue();
  }

  inline void giveTestValue(int* testValue) noexcept {
    impl<Impl>()->giveTestValue(testValue);
  }

  [[nodiscard]] inline Impl* getImplPointer() const noexcept {
    return impl<Impl>();
  }
};

bool TestSharedObject::Impl::implementationDeleted = false;

TestSharedObject::TestSharedObject() noexcept
    : ::yaul::SharedObject(std::make_shared<TestSharedObject::Impl>()) {}

YAUL_IMPL_DESTRUCT(TestSharedObject);
YAUL_IMPL_MOVE(TestSharedObject, ::yaul::SharedObject);
YAUL_IMPL_COPY_SHARED(TestSharedObject);

TestSharedObject::Impl::~Impl() noexcept {
  EXPECT_FALSE(implementationDeleted);
  delete testValue;
  implementationDeleted = true;
}

TestSharedObject::Impl::Impl(TestSharedObject::Impl&& o) noexcept {
  std::swap(testValue, o.testValue);
}

TestSharedObject::Impl& TestSharedObject::Impl::operator=(
    TestSharedObject::Impl&& o) noexcept {
  std::swap(testValue, o.testValue);
  return *this;
}

TestSharedObject::Impl::Impl(const TestSharedObject::Impl& o) noexcept
    : testValue(new int(*o.testValue)) {}
TestSharedObject::Impl& TestSharedObject::Impl::operator=(
    const TestSharedObject::Impl& o) noexcept {
  delete testValue;
  testValue = new int(*o.testValue);
  return *this;
}

class SharedObject : public ::testing::Test {
 protected:
  ::yaul::string initialConfig;

  int testValue;
  TestSharedObject* object;

  virtual void SetUp() {
    testValue = rand();
    object    = new TestSharedObject();
    object->giveTestValue(new int(testValue));

    initialConfig += "  testValue   = " + std::to_string(testValue) + ";\n";
  }

  virtual void TearDown() {
    delete object;
    EXPECT_TRUE(TestSharedObject::Impl::implementationDeleted);

    if (::testing::Test::HasFailure()) {
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_RED,
                                         "Failing Test Configuration\n");
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_DEFAULT, "%s",
                                         initialConfig.c_str());
    }
  }
};

TEST_F(SharedObject, Constructor) {
  ASSERT_TRUE(object->implSet());
  ASSERT_NE(object->getTestValue(), nullptr);
  EXPECT_EQ(*object->getTestValue(), testValue);
}

TEST_F(SharedObject, CopyConstructor) {
  {
    TestSharedObject copy(*object);

    ASSERT_TRUE(object->implSet());
    ASSERT_NE(object->getTestValue(), nullptr);

    ASSERT_TRUE(copy.implSet());
    ASSERT_NE(copy.getTestValue(), nullptr);

    // Implementation should be the same
    ASSERT_EQ(object->getImplPointer(), copy.getImplPointer());

    ASSERT_EQ(object->getTestValue(), copy.getTestValue());

    ASSERT_EQ(*object->getTestValue(), *copy.getTestValue());
  }
  // copy gets deleted when it goes out of scope but the implementation should
  // not get deleted because it is shared and object is still an owner
  EXPECT_FALSE(TestSharedObject::Impl::implementationDeleted);
}

TEST_F(SharedObject, CopyAssignment) {
  {
    TestSharedObject copy(*object);
    copy = *object;

    ASSERT_TRUE(object->implSet());
    ASSERT_NE(object->getTestValue(), nullptr);

    ASSERT_TRUE(copy.implSet());
    ASSERT_NE(copy.getTestValue(), nullptr);

    // Implementation should be the same
    ASSERT_EQ(object->getImplPointer(), copy.getImplPointer());

    ASSERT_EQ(object->getTestValue(), copy.getTestValue());

    ASSERT_EQ(*object->getTestValue(), *copy.getTestValue());
  }
  // copy gets deleted when it goes out of scope but the implementation should
  // not get deleted because it is shared and object is still an owner
  EXPECT_FALSE(TestSharedObject::Impl::implementationDeleted);
}

TEST_F(SharedObject, MoveConstructor) {
  {
    TestSharedObject destination(std::move(*object));
    ASSERT_FALSE(object->implSet());
    ASSERT_TRUE(destination.implSet());
    EXPECT_FALSE(TestSharedObject::Impl::implementationDeleted);

    ASSERT_NE(destination.getTestValue(), nullptr);

    ASSERT_EQ(*destination.getTestValue(), testValue);
  }
  // destination gets deleted when it goes out of scope and the implementation
  // should get deleted because it is shared but object is no longer an owner
  EXPECT_TRUE(TestSharedObject::Impl::implementationDeleted);
}

TEST_F(SharedObject, MoveAssignment) {
  {
    TestSharedObject destination(*object);
    destination = std::move(*object);
    ASSERT_FALSE(object->implSet());
    ASSERT_TRUE(destination.implSet());
    EXPECT_FALSE(TestSharedObject::Impl::implementationDeleted);

    ASSERT_NE(destination.getTestValue(), nullptr);

    ASSERT_EQ(*destination.getTestValue(), testValue);
  }
  // destination gets deleted when it goes out of scope and the implementation
  // should get deleted because it is shared but object is no longer an owner
  EXPECT_TRUE(TestSharedObject::Impl::implementationDeleted);
}
