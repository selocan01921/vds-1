#ifndef __VDS_CORE_FOLDERNAME_H_
#define __VDS_CORE_FOLDERNAME_H_

/*
Copyright (c) 2017, Vadim Malyshev, lboss75@gmail.com
All rights reserved
*/

namespace vds {
  class filename;
  
  class foldername
  {
  public:
      foldername()
      {
      }
      
      foldername(const foldername& other)
      : value_(other.value_)
      {
      }
      
      foldername(const std::string & str)
      : value_(str)
      {
      }
      
      foldername(
        const foldername& base,
        const std::string & relative);
      
      foldername& operator=(const foldername& other)
      {      
        this->value_ = other.value_;
        return *this;
      }
      
      bool operator==(const foldername& other) const
      {
        return this->value_ == other.value_;
      }
      
      const std::string & str() const {
        return this->value_;
      }
      
      void folders(
        const std::function<bool(const foldername & name)> & callback
      ) const;

      void files(
        const std::function<bool(const filename & name)> & callback
      ) const;

      std::string relative_path(const filename & fn, bool allow_pass_border = false) const;
      std::string name() const;

  private:
    friend class filename;
    std::string value_;
  };
}

#endif // __VDS_CORE_FOLDERNAME_H_