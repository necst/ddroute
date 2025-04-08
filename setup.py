from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "ddroute_cpp",
        ["src/ddroute.cpp"],
        include_dirs=[pybind11.get_include()],
        extra_compile_args=["-O3","-shared"],
        language="c++"
    )
]

setup(
    name="ddroute",
    packages=["ddroute"],
    version="0.1.0",
    install_requires=["numpy","qiskit"],
    python_requires='>=3.9',
    ext_modules=ext_modules,
    zip_safe=False,
)
