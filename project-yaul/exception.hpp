// class YAUL_API Exception : public ::std::exception {
//  public:
//   /**
//    * @brief Construct a new Exception object with empty message
//    *
//    */
//   Exception() {}

//   /**
//    * @brief Construct a new Exception object
//    *
//    * @param msg of the exception
//    */
//   Exception(const char* msg);

//   /**
//    * @brief Construct a new Exception object
//    *
//    * @param msg of the exception
//    * @param errNum error number
//    */
//   Exception(const char* msg, int errNum) {
//     const size_t bufferSize = 250;
//     char buf[bufferSize]    = "";
//     char* systemMessage     = buf;
//     if (strerror_s(buf, bufferSize, errNum) != 0) {
//       systemMessage = strerror(errNum);
//     }
//     this->msg = msg + ": " + systemMessage;
//   }

//   /**
//    * @brief Get exception explanation message
//    *
//    * @return const char*
//    */
//   const char* what() const noexcept override { return msg }

//   inline bool isFailed() const noexcept { return pImpl == nullptr; }

//  private:
//   class Impl;
//   Impl* pImpl = nullptr;
// };
