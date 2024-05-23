#include <wx/wx.h>
#include <main.h>
#include <app.h>
#include <jni.h>

class JLauncher : public wxApp {
public:
    int OnRun() override;

private:
    static void ShowError(const std::string&);
};

wxIMPLEMENT_APP(JLauncher);

int JLauncher::OnRun() {
    // Define parameters
    std::string jre = JRE;
    std::string jvmLib = jre + JVMLIB;
    std::string jar = JAR;
    std::string mainClass = MAIN_CLASS;

    // Declare pointers to JVM and JNI environment
    JavaVM* jvm;
    JNIEnv* env;

    // Load JVM library
    LIBTYPE hJVMLIB = OPENLIB(jvmLib.c_str());
    if (!hJVMLIB) {
        ShowError("Failed to load library: " + jvmLib);
        return -1;
    }

    // Redefine JNI_CreateJavaVM to use loaded jvm.dll
    typedef jint(JNICALL* fpCJV)(JavaVM**, void**, void*);
    auto JNI_CreateJavaVM = reinterpret_cast<fpCJV>(GetProcAddress(hJVMLIB, "JNI_CreateJavaVM"));

    // Set JVM options
    JavaVMOption options[2];
    std::string javaHome = "-Djava.home=" + jre;
    options[0].optionString = javaHome.data();
    std::string classPath = "-Djava.class.path=lib/" + jar;
    options[1].optionString = classPath.data();

    // Set JVM initialization arguments
    JavaVMInitArgs vm_args;
    vm_args.version = JNI_VERSION;
    vm_args.nOptions = 2;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = JNI_FALSE;

    // Create the JVM
    jint res = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), &vm_args);
    if (res != JNI_OK) {
        ShowError("Failed to create JVM: " + res);
        return -1;
    }

    // Find the main class
    jclass mainClassObj = env->FindClass(mainClass.c_str());
    if (!mainClassObj) {
        ShowError("Failed to find main class: " + mainClass);
        jvm->DestroyJavaVM();
        return -1;
    }

    // Find the main method
    jmethodID mainMethod = env->GetStaticMethodID(mainClassObj, "main", "([Ljava/lang/String;)V");
    if (!mainMethod) {
        ShowError("Failed to find main method");
        jvm->DestroyJavaVM();
        return -1;
    }

    // Prepare the argument array (empty in this case)
    jobjectArray args = env->NewObjectArray(0, env->FindClass("java/lang/String"), nullptr);

    // Call the main method
    env->CallStaticVoidMethod(mainClassObj, mainMethod, args);

    // Destroy the JVM
    jvm->DestroyJavaVM();

    return 0;
}

void JLauncher::ShowError(const std::string& message) {
    auto* dial = new wxMessageDialog(nullptr, message.c_str(), APP_NAME, wxOK | wxICON_ERROR);
    dial->ShowModal();
}
