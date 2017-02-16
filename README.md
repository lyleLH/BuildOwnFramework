# BuildOwnFramework

###说明
参照raywenderlich网站上的文章来将自己的代码文件打包成可复用的静态framework

[原文链接：How to Create a Framework for iOS - Sam Davies posted on raywenderlich.com](https://www.raywenderlich.com/65964/create-a-framework-for-ios)

（内网找到的一些资料在操作性上有各种问题，也可能是我漏掉了一些步骤，对个人格言只具有一定的参考意义，通过英文原文的方法才成功实践了一次）

现在将整个过程做一个精简的记录，方便以后快速再次实践

###1.准备好你所有的要打包的代码文件，给你的静态库取一个名字如示例中的RWUIControls
###2.创建一个Xcode静态库工程，命名为RWUIControls
	
	File\New\Project --> iOS\Framework and Library\Cocoa Touch Static Library
- 创建完成之后会自动生成两个文件 `RWUIControls.h`和`RWUIControls.m`两个文件，将`RWUIControls.m`删除

- 保留的`RWUIControls.h`文件用来引用你所有需要暴露出来的其他头文件，示例是做一个UI库，所以需要`#import <UIKit/UIKit.h>`这个基础库(`Foundation`库不需要额外添加，原文图片中有这个库不应该引起不必要的误会)
	
- 在工程中选中后静态库Target，选中`Build Phases `，点击`Link Binary with Libraries`区域展开，然后添加`UIKit.framework`

>一个静态库必须要配合对应的头文件才是有用的，所以在编译配置中需要加入这些你要暴露出去的头文件

- 同样在静态库Target下，选中`Build Phases `，然后点击Xcode顶部菜单的 `Editor\Add Build Phase\Add Copy Headers Build Phase `

	（如果你发现`Add Copy Headers Build Phase`这个菜单不可旋，需要你用鼠标点击`Build Phases` 面板之外的空白区域，取消焦点后再试一次）
- `Copy Headers`树下有三个子树，将你要暴露的头文件全部拖到`public`树下（默认是`project`树下），示例中是`RWUIControls.h`和`RWKnobControl.h`

- 然后将`RWUIControls.h`替换为以下代码

 ```
 // Knob Control
#import <RWUIControls/RWKnobControl.h>
 ```
 
###3.编译配置