# BuildOwnFramework

>注意事项
>依赖工程使用archive时需要注意header search path，
>直接使用framwork archive的时候需要注意bitcode

## 说明

参照raywenderlich网站上的文章来将自己的代码文件打包成可复用的静态framework

[原文链接：How to Create a Framework for iOS - Sam Davies posted on raywenderlich.com](https://www.raywenderlich.com/65964/create-a-framework-for-ios)

（内网找到的一些资料在操作性上有各种问题，也可能是我漏掉了一些步骤，对我只发挥了一定的参考意义，后来通过英文原文的方法才成功实践了一次）

现在将整个过程做一个精简的记录，方便以后快速再次实践

## Part 1

>先生成传统的`.a`静态库

### 1.准备好你所有的要打包的代码文件，给你的静态库取一个名字如示例中的RWUIControls

### 2.创建一个Xcode静态库工程，命名为RWUIControls
	
	File\New\Project --> iOS\Framework and Library\Cocoa Touch Static Library
- 创建完成之后会自动生成两个文件 `RWUIControls.h`和`RWUIControls.m`两个文件，将`RWUIControls.m`删除

- 保留的`RWUIControls.h`文件用来引用你所有需要暴露出来的其他头文件，示例是做一个UI库，所以需要`#import <UIKit/UIKit.h>`这个基础库(`Foundation`库不需要额外添加，原文图片中有这个库不应该引起不必要的误会)
	
- 在工程中选中后静态库Target，选中`Build Phases `，点击`Link Binary with Libraries`区域展开，然后添加`UIKit.framework`



- 同样在静态库Target下，选中`Build Phases `，然后点击Xcode顶部菜单的 `Editor\Add Build Phase\Add Copy Headers Build Phase `

	 (一个静态库必须要配合对应的头文件才是有用的，所以在编译配置中需要加入这些你要暴露出去的头文件)
	 
	（如果你发现`Add Copy Headers Build Phase`这个菜单不可旋，那么需要你用鼠标点击`Build Phases` 面板之外的空白区域，这样是为了取消焦点，然后再试一次）
- `Copy Headers`树下有三个子树，将你要暴露的头文件全部拖到`public`树下（默认是`project`树下），示例中是`RWUIControls.h`和`RWKnobControl.h`

- 然后将`RWUIControls.h`替换为以下代码

 ```
 // Knob Control
#import <RWUIControls/RWKnobControl.h>
 ```
 
### 3.编译配置

- 接下来设置好这个静态库的所有暴露的头文件的文件夹路径，`Build Settings`送搜索`Public Headers Folder Path`，然后设置路径参数为`include/$(PROJECT_NAME)`

- 设置一些二进制库的选项，让编译器去移除一些无用代码，（在`Build Settings `中用搜索找到这些设置）：
	- 	Dead Code Stripping – Set this to NO
	- Strip Debug Symbols During Copy – Set this to NO for all configurations
	- Strip Style – Set this to Non-Global Symbols

### 4.编译然后确认是否生成

- 选择静态库这个target 编译，之后可以看到`Products `组下的`libRWUIControls.a`文件由之前的红色变为黑色
- 选中`libRWUIControls.a`右键在finder中查看，`libRWUIControls.a`同级目录下会有一个`include`文件夹，里面包含了你要暴露出来的头文件


## Part 2

>将这个静态库工程应用为你的开发项目的依赖工程的实例，这样的好处是，在开发项目中能随时修改静态库中的代码并编译后作为主工程的依赖被使用

### 1.创建一个单应用程序工程命名为`UIControlDevApp`（先关闭之前的的`RWUIControls`工程）
### 2.在Finder中找到`RWUIControls.xcodeproj`文件夹，拖到`UIControlDevApp`中
### 3.将之前应用的业务代码`DevApp `拖到`UIControlDevApp`中，来保证对这个UI库的效果在运行之后能被看到
###4.现在来保证`UIControlDevApp`程序能够将静态库作为编译依赖，即
-	选中`UIControlDevApp `Target 导航到`Build Phases`标签下
-  打开` Target Dependencies` 树，点击` +`来选择 `RWUIControls`静态库
-  打开`Link Binary With Libraries `,点击` +`来选择 `Workspace`组下的 `libUIControls.a`（确保Xcode能让你的项目链接到这个静态库就像链接到`UIKit`一样）

### 5.Build and Run

## Part 3

>先生成framework形式的的静态库,这样一来在别的其他工程中，你只需要将这个`xxx.framework`直接拖到工程中就能使用

### 1.Framework Structure的结构

- [Framework Structure的结构图](https://koenig-media.raywenderlich.com/uploads/2014/02/ios_framework_directory_structure.png)

### 2.增加framework编译过程中的脚本
- 选中`RWUIControls`target，导航到` Build Phases` 在Xcode顶部菜单中选择`Editor/Add Build Phase/Add Run Script Build Phase`新增一个`Run Script`树，改名字为`Build Framework`,粘贴以下bash脚本:

```
set -e
 
export FRAMEWORK_LOCN="${BUILT_PRODUCTS_DIR}/${PRODUCT_NAME}.framework"
 
# Create the path to the real Headers die
mkdir -p "${FRAMEWORK_LOCN}/Versions/A/Headers"
 
# Create the required symlinks
/bin/ln -sfh A "${FRAMEWORK_LOCN}/Versions/Current"
/bin/ln -sfh Versions/Current/Headers "${FRAMEWORK_LOCN}/Headers"
/bin/ln -sfh "Versions/Current/${PRODUCT_NAME}" \
             "${FRAMEWORK_LOCN}/${PRODUCT_NAME}"
 
# Copy the public headers into the framework
/bin/cp -a "${TARGET_BUILD_DIR}/${PUBLIC_HEADERS_FOLDER_PATH}/" \
           "${FRAMEWORK_LOCN}/Versions/A/Headers"
           
           

```
### 3.Build

### 4.检查
- 在`Products`中选中`libRWUIControls.a` 右键在finder中查看，可以看到`RWUIControls.framework`中的目录是否和之前给的framework目录结构一致，但是会发现`Version/A`目录下没有`.a`文件，所以尚未完成
- 因为这个静态库文件要根据你的工程所运行的平台来编译，模拟器和真机是不一样的，要选择不同的架构编译之后才能生成对应可用的静态库
### 5.多架构编译，创建多平台下可用的聚合体Framework
- 选择`RWUIControls `target，在`Project Navigator `下点击` Add Target `，在弹出面板中找到`iOS/Other/Aggregate`，选择后点击下一步，将这个聚合体命名为`Framework `
- 选择`Framework `target，导航到`Build Phases `面板下，展开`Target Dependencies`树
，点击`+`选中`RWUIControls `静态库
- 为这个聚合体增加指定多平台编译的脚本，在顶部菜单中选择`Editor/Add Build Phase/Add Run Script Build Phase`，然后将新生成的`Run Script`树，命名为`MultiPlatform Build`,然后添加以下bash脚本

```
	set -e
	 # If we're already inside this script then die
	if [ -n "$RW_MULTIPLATFORM_BUILD_IN_PROGRESS" ]; then
	  exit 0
	fi
	export RW_MULTIPLATFORM_BUILD_IN_PROGRESS=1
 
	RW_FRAMEWORK_NAME=${PROJECT_NAME}
	RW_INPUT_STATIC_LIB="lib${PROJECT_NAME}.a"
	RW_FRAMEWORK_LOCATION="${BUILT_PRODUCTS_DIR}/${RW_FRAMEWORK_NAME}.framework"
	
	
	function build_static_library {
    # Will rebuild the static library as specified
    #     build_static_library sdk
    xcrun xcodebuild -project "${PROJECT_FILE_PATH}" \
                     -target "${TARGET_NAME}" \
                     -configuration "${CONFIGURATION}" \
                     -sdk "${1}" \
                     ONLY_ACTIVE_ARCH=NO \
                     BUILD_DIR="${BUILD_DIR}" \
                     OBJROOT="${OBJROOT}" \
                     BUILD_ROOT="${BUILD_ROOT}" \
                     SYMROOT="${SYMROOT}" $ACTION
}
 
function make_fat_library {
    # Will smash 2 static libs together
    #     make_fat_library in1 in2 out
    xcrun lipo -create "${1}" "${2}" -output "${3}"
}

# 1 - Extract the platform (iphoneos/iphonesimulator) from the SDK name
if [[ "$SDK_NAME" =~ ([A-Za-z]+) ]]; then
  RW_SDK_PLATFORM=${BASH_REMATCH[1]}
else
  echo "Could not find platform name from SDK_NAME: $SDK_NAME"
  exit 1
fi
 
	# 2 - Extract the version from the SDK
if [[ "$SDK_NAME" =~ ([0-9]+.*$) ]]; then
  RW_SDK_VERSION=${BASH_REMATCH[1]}
else
  echo "Could not find sdk version from SDK_NAME: $SDK_NAME"
  exit 1
fi
 
	# 3 - Determine the other platform
if [ "$RW_SDK_PLATFORM" == "iphoneos" ]; then
  RW_OTHER_PLATFORM=iphonesimulator
else
  RW_OTHER_PLATFORM=iphoneos
fi
 
# 4 - Find the build directory
if [[ "$BUILT_PRODUCTS_DIR" =~ (.*)$RW_SDK_PLATFORM$ ]]; then
  RW_OTHER_BUILT_PRODUCTS_DIR="${BASH_REMATCH[1]}${RW_OTHER_PLATFORM}"
else
  echo "Could not find other platform build directory."
  exit 1
fi


# Build the other platform.
build_static_library "${RW_OTHER_PLATFORM}${RW_SDK_VERSION}"
 
# If we're currently building for iphonesimulator, then need to rebuild
#   to ensure that we get both i386 and x86_64
if [ "$RW_SDK_PLATFORM" == "iphonesimulator" ]; then
    build_static_library "${SDK_NAME}"
fi
 
# Join the 2 static libs into 1 and push into the .framework
make_fat_library "${BUILT_PRODUCTS_DIR}/${RW_INPUT_STATIC_LIB}" \
                 "${RW_OTHER_BUILT_PRODUCTS_DIR}/${RW_INPUT_STATIC_LIB}" \
                 "${RW_FRAMEWORK_LOCATION}/Versions/A/${RW_FRAMEWORK_NAME}"
                 
                 
                 # Ensure that the framework is present in both platform's build directories
cp -a "${RW_FRAMEWORK_LOCATION}/Versions/A/${RW_FRAMEWORK_NAME}" \
      "${RW_OTHER_BUILT_PRODUCTS_DIR}/${RW_FRAMEWORK_NAME}.framework/Versions/A/${RW_FRAMEWORK_NAME}"
 
# Copy the framework to the user's desktop
ditto "${RW_FRAMEWORK_LOCATION}" "${HOME}/Desktop/${RW_FRAMEWORK_NAME}.framework"

```
### 6.编译和结果检查

- 这一次编译将会生成一个`RWUIControls.framework`在你的桌面上

- 检查是否有效生成

```
	cd ~/Desktop/RWUIControls.framework
	$ RWUIControls.framework  xcrun lipo -info RWUIControls
```



## Part4 
### 如何使用Framework

- 创建一个新的单视图应用程序工程
- 将桌面上的Framework 拖拽到工程中，拖拽时确保选择`Copy items into destination group’s folder`
- 添加业务代码 就能运行看到使用效果
