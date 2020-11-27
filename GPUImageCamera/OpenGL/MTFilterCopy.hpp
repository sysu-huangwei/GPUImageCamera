//
//  MTFilterCopy.hpp
//  GPUImageCamera
//
//  Created by rayyy on 2020/11/26.
//  Copyright © 2020 meitu. All rights reserved.
//

#ifndef MTFilterCopy_hpp
#define MTFilterCopy_hpp

#include "MTFilterBase.hpp"

class MTFilterCopy : public MTFilterBase {
public:
    MTFilterCopy();
    ~MTFilterCopy();
    virtual void init() override;
    virtual unsigned render() override;
    
    void setSrcTextureID(unsigned srcTextureID);
    
private:
    int inputImageTextureUniform;
    unsigned srcTextureID;
};

#endif /* MTFilterCopy_hpp */
