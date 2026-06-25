#ifndef ApiBase_H_
#define ApiBase_H_

#include <memory>

#include <pistache/router.h>

namespace api {

class ApiBase {
  public:
    explicit ApiBase(const std::shared_ptr<Pistache::Rest::Router> &rtr);
    virtual ~ApiBase() = default;
    virtual void init() = 0;

  protected:
    const std::shared_ptr<Pistache::Rest::Router> router;
};

} // namespace api

#endif